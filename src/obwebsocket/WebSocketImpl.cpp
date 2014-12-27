#include "WebSocketImpl.h"

#include "../openblox/OpenBlox.h"

#ifdef _WIN32
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT  0x501
	#endif
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <sys/types.h>
    #include <io.h>

    #define socketerrno WSAGetLastError()
    #define SOCKET_EAGAIN_EINPROGRESS WSAEINPROGRESS
    #define SOCKET_EWOULDBLOCK WSAEWOULDBLOCK
#else
    #include <netdb.h>
    #include <netinet/tcp.h>
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <stdint.h>

    #include <errno.h>
    #define socketerrno errno
    #define SOCKET_EAGAIN_EINPROGRESS EAGAIN
    #define SOCKET_EWOULDBLOCK EWOULDBLOCK
#endif

namespace OpenBlox{
	WebSocketImpl::WebSocketImpl(const char* url){
		this->url = url;
		initialized = false;

		readyState = CONNECTING;

		socketfd = 0;

		ssl = NULL;
		ssl_ctx = NULL;
		bio = NULL;
	}

	WebSocketImpl::~WebSocketImpl(){}

	bool WebSocketImpl::isInitialized(){
		return initialized;
	}

	void WebSocketImpl::init(){
		if(strlen(url) >= 128){
			LOGE("URL size limit exceeded: %s", url); //TODO: Change this to an OnError call
			return;
		}

		char host[128];
		int port;
		char path[128];

		bool useSSL = false;
		if(sscanf(url, "wss://%[^:/]:%d/%s", host, &port, path) == 3){
			useSSL = true;
		}else if(sscanf(url, "wss://%[^:/]/%s", host, path) == 2){
			useSSL = true;
			port = 443;
		}else if(sscanf(url, "wss://%[^:/]:%d", host, &port) == 2){
			useSSL = true;
			path[0] = '\0';
		}else if(sscanf(url, "wss://%[^:/]", host) == 1){
			useSSL = true;
			port = 443;
			path[0] = '\0';
		}else if(sscanf(url, "ws://%[^:/]:%d/%s", host, &port, path) == 3){
		}else if(sscanf(url, "ws://%[^:/]/%s", host, path) == 2){
			port = 80;
		}else if(sscanf(url, "ws://%[^:/]:%d", host, &port) == 2){
			path[0] = '\0';
		}else if(sscanf(url, "ws://%[^:/]", host) == 1){
			port = 80;
			path[0] = '\0';
		}else{
			LOGE("Failed to parse URL: %s", url); //TODO: Change this to an OnError call
			return;
		}

		LOGI("Connecting: host=%s port=%d path=/%s", host, port, path);

		struct hostent *hostaddr;
		struct sockaddr_in saiServerAddress;

		hostaddr = gethostbyname(host);
		socketfd = socket(AF_INET, SOCK_STREAM, 0);
		if(socketfd < 0){
			LOGI("Unable to create socket");
			return;
		}

		saiServerAddress.sin_family = AF_INET;
		saiServerAddress.sin_port = htons(port);
		saiServerAddress.sin_addr = *((struct in_addr*)hostaddr->h_addr);
		memset(&(saiServerAddress.sin_zero), 0, 8);

		int err = connect(socketfd, (struct sockaddr*)&saiServerAddress, sizeof(saiServerAddress));
		if(err == -1){
			LOGI("Unable to connect"); //TODO: Change this to an OnError call
			return;
		}

		int flag = 1;
		setsockopt(socketfd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));

		if(useSSL){
			ssl_ctx = SSL_CTX_new(SSLv23_client_method());
			if(ssl_ctx == NULL){
				LOGI("SSL Error: %s", ERR_error_string(ERR_get_error(), NULL)); //TODO: Change this to an OnError call
				return;
			}

			ssl = SSL_new(ssl_ctx);
			if(ssl == NULL){
				LOGI("SSL Error: %s", ERR_error_string(ERR_get_error(), NULL)); //TODO: Change this to an OnError call
				return;
			}

			SSL_set_verify(ssl, SSL_VERIFY_NONE, NULL);

			if(!SSL_set_fd(ssl, socketfd)){
				LOGI("SSL Error: %s", ERR_error_string(ERR_get_error(), NULL)); //TODO: Change this to an OnError call
				return;
			}

			if(SSL_connect(ssl) != 1){
				LOGI("SSL Error: %s", ERR_error_string(ERR_get_error(), NULL)); //TODO: Change this to an OnError call
				return;
			}
		}else{
			bio = BIO_new_socket(socketfd, BIO_NOCLOSE);
			if(BIO_do_connect(bio) < 0){
				LOGI("BIO Error: %s", ERR_error_string(ERR_get_error(), NULL)); //TODO: Change this to an OnError call
				return;
			}
		}

		char line[256];
		int status;
		int i;

		snprintf(line, 256, "GET /%s HTTP/1.1\r\n", path);
		write(line);

		if(port == 80){
			snprintf(line, 256, "Host: %s\r\n", host);
		}else{
			snprintf(line, 256, "Host: %s:%d\r\n", host, port);
		}
		write(line);

		snprintf(line, 256, "Upgrade: websocket\r\n");
		write(line);

		snprintf(line, 256, "Connection: Upgrade\r\n");
		write(line);

		snprintf(line, 256, "Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\r\n");
		write(line);

		snprintf(line, 256, "Sec-WebSocket-Version: 13\r\n");
		write(line);

		snprintf(line, 256, "\r\n");
		write(line);

		for(i = 0; i < 2 || (i < 255 && line[i-2] != '\r' && line[i-1] != '\n'); ++i){
			if(read(line+i, 1) == 0){
				return;
			}
		}
		line[i] = 0;
		if(i == 255){
			LOGI("Invalid status line"); //TODO: Change this to an OnError call
			return;
		}
		if(sscanf(line, "HTTP/1.1 %d", &status) != 1 || status != 101){
			LOGI("Bad status %d", status); //TODO: Change this to an OnError call
			return;
		}
		while(true){
			for(i = 0; i < 2 || (i < 255 && line[i-2] != '\r' && line[i-1] != '\n'); ++i){
				if(read(line+i, 1) == 0){
					return;
				}
			}
			if(line[0] == '\r' && line[1] == '\n'){
				break;
			}
		}

		initialized = true;
	}

	WebSocketImpl::readyStateValues WebSocketImpl::getReadyState(){
		return readyState;
	}

	void WebSocketImpl::poll(int timeout){
		if(readyState == CLOSED){
			if(timeout > 0){
				timeval tv = { timeout/1000, (timeout%1000) * 1000 };
				select(0, NULL, NULL, NULL, &tv);
			}
		}
		if(timeout > 0){
			fd_set rfds;
			fd_set wfds;
			timeval tv = { timeout/1000, (timeout%1000) * 1000 };
			FD_ZERO(&rfds);
			FD_ZERO(&wfds);
			FD_SET(socketfd, &rfds);
			if(txbuf.size()){
				FD_SET(socketfd, &wfds);
			}
			select(socketfd + 1, &rfds, &wfds, NULL, &tv);
		}
		LOGI("AFTER SELECT");
		while(true){
			int N = rxbuf.size();
			LOGI("N %d", N);
			ssize_t ret;
			rxbuf.resize(N + 1500);
			ret = emulate_recv(&rxbuf[0] + N, 1500);
			LOGI("ret %d", ret);
			if(ret < 0 && (socketerrno == SOCKET_EWOULDBLOCK || socketerrno == SOCKET_EAGAIN_EINPROGRESS)){
				rxbuf.resize(N + ret);
				break;
			}else if(ret <= 0){
				rxbuf.resize(N);
				shutdown();
				readyState = CLOSED;
				LOGI("CLOSED");
				break;
			}else{
				rxbuf.resize(N + ret);
			}
		}
		LOGI("After while");
		while(txbuf.size()){
			int ret = write((char*)&txbuf[0]);
			if(ret < 0 && (socketerrno == SOCKET_EWOULDBLOCK || socketerrno == SOCKET_EAGAIN_EINPROGRESS)){
				break;
			}else if(ret <= 0){
				shutdown();
				readyState = CLOSED;
				LOGI("CLOSED");
				break;
			}else{
				txbuf.erase(txbuf.begin(), txbuf.begin() + ret);
			}
		}
		if(!txbuf.size() && readyState == CLOSING){
			shutdown();
			readyState = CLOSED;
			LOGI("CLOSED AT POLL END");
		}
	}

	void WebSocketImpl::send(const char* msg){
		const std::string message = std::string(msg);
		sendData(TEXT_FRAME, message.size(), message.begin(), message.end());
	}

	template<class Iterator> void WebSocketImpl::sendData(opcode_type type, uint64_t message_size, Iterator message_begin, Iterator message_end){
		const uint8_t masking_key[4] = { 0x12, 0x34, 0x56, 0x78 };
		// TODO: consider acquiring a lock on txbuf

		bool useMask = false;

		if (readyState == CLOSING || readyState == CLOSED){
			return;
		}
		std::vector<uint8_t> header;
		header.assign(2 + (message_size >= 126 ? 2 : 0) + (message_size >= 65536 ? 6 : 0) + (useMask ? 4 : 0), 0);
		header[0] = 0x80 | type;
		if(message_size < 126){
			header[1] = (message_size & 0xff) | (useMask ? 0x80 : 0);
			if (useMask) {
				header[2] = masking_key[0];
				header[3] = masking_key[1];
				header[4] = masking_key[2];
				header[5] = masking_key[3];
			}
		}else if (message_size < 65536){
			header[1] = 126 | (useMask ? 0x80 : 0);
			header[2] = (message_size >> 8) & 0xff;
			header[3] = (message_size >> 0) & 0xff;
			if(useMask){
				header[4] = masking_key[0];
				header[5] = masking_key[1];
				header[6] = masking_key[2];
				header[7] = masking_key[3];
			}
		}else{
			header[1] = 127 | (useMask ? 0x80 : 0);
			header[2] = (message_size >> 56) & 0xff;
			header[3] = (message_size >> 48) & 0xff;
			header[4] = (message_size >> 40) & 0xff;
			header[5] = (message_size >> 32) & 0xff;
			header[6] = (message_size >> 24) & 0xff;
			header[7] = (message_size >> 16) & 0xff;
			header[8] = (message_size >>  8) & 0xff;
			header[9] = (message_size >>  0) & 0xff;
			if(useMask){
				header[10] = masking_key[0];
				header[11] = masking_key[1];
				header[12] = masking_key[2];
				header[13] = masking_key[3];
			}
		}
		// N.B. - txbuf will keep growing until it can be transmitted over the socket:
		txbuf.insert(txbuf.end(), header.begin(), header.end());
		txbuf.insert(txbuf.end(), message_begin, message_end);
		if(useMask){
			for (size_t i = 0; i != message_size; ++i) { *(txbuf.end() - message_size + i) ^= masking_key[i&0x3]; }
		}
	}

	void WebSocketImpl::closeSocket(){
		if(readyState == CLOSING || readyState == CLOSED){
			return;
		}
		readyState = CLOSING;
		uint8_t closeFrame[6] = {0x88, 0x80, 0x00, 0x00, 0x00, 0x00}; // last 4 bytes are a masking key
		std::vector<uint8_t> header(closeFrame, closeFrame+6);
		txbuf.insert(txbuf.end(), header.begin(), header.end());
	}

	void WebSocketImpl::dispatch(OnMessageCallback* cb){
		LOGI("Dispatch");
		struct _Callback : public BytesCallback_Imp{
			OnMessageCallback* callable;
			_Callback(OnMessageCallback* callable) : callable(callable){}
			void operator()(const std::vector<uint8_t>& message, WebSocketImpl* ws){
				std::string stringMessage(message.begin(), message.end());
				LOGI("Called");
				callable(stringMessage.c_str(), ws);
			}
		};
		_Callback callback(cb);
		_dispatchBinary(callback);
	}

	void WebSocketImpl::_dispatchBinary(BytesCallback_Imp & callable){
		// TODO: consider acquiring a lock on rxbuf
		while(true){
			wsheader_type ws;
			if (rxbuf.size() < 2){
				return;
			}
			const uint8_t * data = (uint8_t *)&rxbuf[0];
			ws.fin = (data[0] & 0x80) == 0x80;
			ws.opcode = (opcode_type)(data[0] & 0x0f);
			ws.mask = (data[1] & 0x80) == 0x80;
			ws.N0 = (data[1] & 0x7f);
			ws.header_size = 2 + (ws.N0 == 126? 2 : 0) + (ws.N0 == 127? 8 : 0) + (ws.mask? 4 : 0);
			if (rxbuf.size() < ws.header_size){
				return;
			}
			int i;
			if(ws.N0 < 126){
				ws.N = ws.N0;
				i = 2;
			}else if(ws.N0 == 126){
				ws.N = 0;
				ws.N |= ((uint64_t) data[2]) << 8;
				ws.N |= ((uint64_t) data[3]) << 0;
				i = 4;
			}else if(ws.N0 == 127){
				ws.N = 0;
				ws.N |= ((uint64_t) data[2]) << 56;
				ws.N |= ((uint64_t) data[3]) << 48;
				ws.N |= ((uint64_t) data[4]) << 40;
				ws.N |= ((uint64_t) data[5]) << 32;
				ws.N |= ((uint64_t) data[6]) << 24;
				ws.N |= ((uint64_t) data[7]) << 16;
				ws.N |= ((uint64_t) data[8]) << 8;
				ws.N |= ((uint64_t) data[9]) << 0;
				i = 10;
			}
			if(ws.mask){
				ws.masking_key[0] = ((uint8_t) data[i+0]) << 0;
				ws.masking_key[1] = ((uint8_t) data[i+1]) << 0;
				ws.masking_key[2] = ((uint8_t) data[i+2]) << 0;
				ws.masking_key[3] = ((uint8_t) data[i+3]) << 0;
			}else{
				ws.masking_key[0] = 0;
				ws.masking_key[1] = 0;
				ws.masking_key[2] = 0;
				ws.masking_key[3] = 0;
			}
			if(rxbuf.size() < ws.header_size+ws.N){
				return;
			}

			// We got a whole message, now do something with it:
			if(ws.opcode == TEXT_FRAME || ws.opcode == BINARY_FRAME || ws.opcode == CONTINUATION){
				if(ws.mask){
					for(size_t i = 0; i != ws.N; ++i){
						rxbuf[i+ws.header_size] ^= ws.masking_key[i&0x3];
					}
				}
				receivedData.insert(receivedData.end(), rxbuf.begin()+ws.header_size, rxbuf.begin()+ws.header_size+(size_t)ws.N);
				if(ws.fin){
					callable((const std::vector<uint8_t>) receivedData, this);
					receivedData.erase(receivedData.begin(), receivedData.end());
					std::vector<uint8_t> ().swap(receivedData);
				}
			}else if(ws.opcode == PING){
				if(ws.mask){
					for(size_t i = 0; i != ws.N; ++i){
						rxbuf[i+ws.header_size] ^= ws.masking_key[i&0x3];
					}
				}
				std::string data(rxbuf.begin()+ws.header_size, rxbuf.begin()+ws.header_size+(size_t)ws.N);
				sendData(PONG, data.size(), data.begin(), data.end());
			}else if(ws.opcode == PONG){}
			else if (ws.opcode == CLOSE){
				closeSocket();
			}else{
				LOGI("Got unexpected WebSocket message.\n");//TODO: Change this to an OnError call
				closeSocket();
			}
			rxbuf.erase(rxbuf.begin(), rxbuf.begin() + ws.header_size+(size_t)ws.N);
		}
	}

	int WebSocketImpl::read(char* buf, size_t len){
		if(ssl != NULL){
			return SSL_read(ssl, buf, len);
		}else{
			return BIO_read(bio, buf, len);
		}
	}

	int WebSocketImpl::emulate_recv(uint8_t* buf, size_t len){
		/*
		if(ssl != NULL){
			return SSL_read(ssl, buf, len);
		}else{
			return BIO_read(bio, buf, len);
		}
		*/
		size_t received = 0;
		int count = 0;
		while(true){
			if(!buf){
				buf = new uint8_t[len];
			}
			if(ssl){
				received = SSL_read(ssl, buf, len);
			}else{
				received = BIO_read(bio, buf, len);
			}
			if(received < len){
				break;
			}
			count++;
		}
		return received;
	}

	int WebSocketImpl::write(char* data){
		if(ssl){
			return SSL_write(ssl, data, strlen(data));
		}else{
			return BIO_write(bio, data, strlen(data));
		}
	}

	void WebSocketImpl::shutdown(){
		close(socketfd);
		if(ssl){
			SSL_shutdown(ssl);
			SSL_free(ssl);
		}else{
			BIO_free_all(bio);
		}
	}
}
