#ifndef OB_TYPE_WEBSOCKET_H_
#define OB_TYPE_WEBSOCKET_H_

#include "LuaEvent.h"

#include "../easywsclient/easywsclient.hpp"

#define lua_websocket_name "luaL_WebSocket"

namespace ob_type{
	class WebSocket{
		public:
			WebSocket(const char* uri);
			virtual ~WebSocket();

			void send(const char* msg);

			static void handleMessage(const std::string& msg, easywsclient::WebSocket::pointer ws);

			DECLARE_STATIC_INIT(WebSocket);

			const char* uri;
			easywsclient::WebSocket::pointer ws;

			static int lua_index(lua_State* L);
			static int lua_newindex(lua_State* L);

			static int lua_toString(lua_State* L);

			int wrap_lua(lua_State* L);
		protected:
			bool inited;

			LuaEvent* OnOpen;
			LuaEvent* OnClose;
			LuaEvent* OnMessage;

			static int lua_send(lua_State* L);
			static int lua_close(lua_State* L);
			static int lua_getReadyState(lua_State* L);
			static int lua_getOnOpen(lua_State* L);
			static int lua_getOnClose(lua_State* L);
			static int lua_getOnMessage(lua_State* L);

			static OpenBlox::Thread* webSocketThread;
			static std::vector<WebSocket*>* activeWebSockets;
	};

	WebSocket* checkWebSocket(lua_State* L, int n);
}
#endif
