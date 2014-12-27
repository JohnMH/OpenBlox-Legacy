// http://tools.ietf.org/html/rfc6455#section-5.2  Base Framing Protocol
//
//  0                   1                   2                   3
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-------+-+-------------+-------------------------------+
// |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
// |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
// |N|V|V|V|       |S|             |   (if payload len==126/127)   |
// | |1|2|3|       |K|             |                               |
// +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
// |     Extended payload length continued, if payload len == 127  |
// + - - - - - - - - - - - - - - - +-------------------------------+
// |                               |Masking-key, if MASK set to 1  |
// +-------------------------------+-------------------------------+
// | Masking-key (continued)       |          Payload Data         |
// +-------------------------------- - - - - - - - - - - - - - - - +
// :                     Payload Data continued ...                :
// + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
// |                     Payload Data continued ...                |
// +---------------------------------------------------------------+

#ifndef OBWEBSOCKET_WEBSOCKETIMPL_H_
#define OBWEBSOCKET_WEBSOCKETIMPL_H_

#include <vector>

#include <stdint.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace OpenBlox{
	class WebSocketImpl;

	typedef void (OnMessageCallback)(const char* msg, WebSocketImpl* ws);

	struct BytesCallback_Imp{
		virtual void operator()(const std::vector<uint8_t>& message, WebSocketImpl* ws) = 0;
	};

	class WebSocketImpl{
		public:
			enum opcode_type{
				CONTINUATION = 0x0,
				TEXT_FRAME = 0x1,
				BINARY_FRAME = 0x2,
				CLOSE = 8,
				PING = 9,
				PONG = 0xa,
			};

			struct wsheader_type{
		        unsigned header_size;
		        bool fin;
		        bool mask;
		        opcode_type opcode;
		        int N0;
		        uint64_t N;
		        uint8_t masking_key[4];
		    };

			enum readyStateValues{
				CONNECTING = 0,
				OPEN = 1,
				CLOSING = 2,
				CLOSED = 3
			};

			WebSocketImpl(const char* url);
			virtual ~WebSocketImpl();

			bool isInitialized();

			void init();

			readyStateValues getReadyState();

			void poll(int timeout = 0);
			void send(const char* msg);
			template<class Iterator> void sendData(opcode_type type, uint64_t message_size, Iterator message_begin, Iterator message_end);
			void closeSocket();

			void dispatch(OnMessageCallback* cb);

			void _dispatchBinary(BytesCallback_Imp& callable);
		protected:
			bool initialized;
			readyStateValues readyState;

			std::vector<uint8_t> rxbuf;
		    std::vector<uint8_t> txbuf;
		    std::vector<uint8_t> receivedData;

			int emulate_recv(uint8_t* buf, size_t len);
			int read(char* buf, size_t len);
			int write(char* data);
			void shutdown();

			int socketfd;
			SSL* ssl;
			SSL_CTX* ssl_ctx;
			BIO* bio;

			const char* url;
	};
};
#endif
