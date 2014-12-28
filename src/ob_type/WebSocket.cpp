#include "WebSocket.h"

namespace ob_type{
	STATIC_INIT(WebSocket){
		lua_State* L = OpenBlox::BaseGame::getGlobalState();

		luaL_newmetatable(L, lua_websocket_name);

		lua_pushstring(L, "__metatable");
		lua_pushstring(L, "This metatable is locked");
		lua_rawset(L, -3);

		lua_pushstring(L, "__methods");
		lua_newtable(L);
		luaL_Reg methods[]{
			{"send", lua_send},
			{"Send", lua_send},
			{"close", lua_close},
			{"Close", lua_close},
			{NULL, NULL}
		};
		luaL_register(L, NULL, methods);
		lua_rawset(L, -3);

		lua_pushstring(L, "__properties");
		lua_newtable(L);
		luaL_Reg props[]{
			{"readyState", lua_getReadyState},
			{"ReadyState", lua_getReadyState},
			{"OnOpen", lua_getOnOpen},
			{"onOpen", lua_getOnOpen},
			{"onopen", lua_getOnOpen},
			{"OnClose", lua_getOnClose},
			{"onClose", lua_getOnClose},
			{"onclose", lua_getOnClose},
			{"OnMessage", lua_getOnMessage},
			{"onMessage", lua_getOnMessage},
			{"onmessage", lua_getOnMessage},
			{NULL, NULL}
		};
		luaL_register(L, NULL, props);
		lua_rawset(L, -3);

		lua_pushstring(L, "__tostring");
		lua_pushcfunction(L, lua_toString);
		lua_rawset(L, -3);

		lua_pushstring(L, "__index");
		lua_pushcfunction(L, lua_index);
		lua_rawset(L, -3);

		lua_pushstring(L, "__newindex");
		lua_pushcfunction(L, lua_newindex);
		lua_rawset(L, -3);

		lua_pop(L, 1);
	}

	OpenBlox::Thread* WebSocket::webSocketThread = NULL;
	std::vector<WebSocket*>* WebSocket::activeWebSockets = NULL;

	WebSocket::WebSocket(const char* uri){
		this->uri = uri;

		inited = false;

		OnOpen = new LuaEvent("OnOpen", 0);
		OnClose = new LuaEvent("OnClose", 0);//0 at this time. 2 expected in the future, long code, char* reason - https://developer.mozilla.org/en-US/docs/Web/API/CloseEvent
		OnMessage = new LuaEvent("OnMessage", 1);

		ws = NULL;

		if(activeWebSockets == NULL){
			activeWebSockets = new std::vector<WebSocket*>();
		}
		activeWebSockets->push_back(this);
		if(webSocketThread == NULL){
			webSocketThread = new OpenBlox::Thread([](){
				while(!activeWebSockets->empty()){
					for(std::vector<WebSocket>::size_type i = 0; i != activeWebSockets->size(); i++){
						WebSocket* ws = (*activeWebSockets)[i];
						if(ws != NULL){
							if(ws->ws == NULL && !ws->inited){
								ws->ws = easywsclient::WebSocket::from_url(std::string(ws->uri));
								if(ws->ws != NULL){
									if(ws->ws->getReadyState() == easywsclient::WebSocket::OPEN){
										std::vector<ob_type::VarWrapper> args = std::vector<ob_type::VarWrapper>();
										ws->OnOpen->Fire(args);
									}
								}
								ws->inited = true;
							}
						}
						if(ws != NULL && ws->ws != NULL && ws->ws->getReadyState() != easywsclient::WebSocket::CLOSED){
							ws->ws->poll();
							ws->ws->dispatch(handleMessage);
						}else{
							if(ws != NULL && ws->ws != NULL){
								std::vector<ob_type::VarWrapper> args = std::vector<ob_type::VarWrapper>();
								ws->OnClose->Fire(args);
								delete ws->ws;
								ws->ws = NULL;
							}
							activeWebSockets->erase(activeWebSockets->begin() + (i - 1));
							break;
						}
					}
				}
				//delete activeWebSockets;
				activeWebSockets = NULL;
				webSocketThread = NULL;
			});
			webSocketThread->start();
		}
	}

	WebSocket::~WebSocket(){
		delete OnClose;
		delete OnMessage;
	}

	void WebSocket::send(const char* msg){
		ws->send(std::string(msg));
	}

	void WebSocket::handleMessage(const std::string& msg, easywsclient::WebSocket::pointer ws){
		for(std::vector<WebSocket>::size_type i = 0; i != activeWebSockets->size(); i++){
			WebSocket* ws = (*activeWebSockets)[i];
			if(ws->ws){
				std::vector<ob_type::VarWrapper> args = std::vector<ob_type::VarWrapper>();
				args.push_back(msg.c_str());

				ws->OnMessage->Fire(args);
				break;
			}
		}
	}

	int WebSocket::lua_send(lua_State* L){
		WebSocket* LuaWebSocket = checkWebSocket(L, 1);
		if(LuaWebSocket != NULL){
			const char* msg = luaL_checkstring(L, 2);
			LuaWebSocket->send(msg);
		}
		return 0;
	}

	int WebSocket::lua_close(lua_State* L){
		WebSocket* LuaWebSocket = checkWebSocket(L, 1);
		if(LuaWebSocket != NULL){
			if(LuaWebSocket->ws){
				LuaWebSocket->ws->close();
			}else{
				luaL_error(L, "WebSocket is already closed.");
			}
		}
		return 0;
	}

	int WebSocket::lua_getReadyState(lua_State* L){
		WebSocket* LuaWebSocket = checkWebSocket(L, 1);
		if(LuaWebSocket != NULL){
			int state = 3;
			if(LuaWebSocket->ws){
				state = LuaWebSocket->ws->getReadyState();
			}else{
				if(!LuaWebSocket->inited){
					lua_pushnumber(L, easywsclient::WebSocket::CONNECTING);
					return 1;
				}
			}
			lua_pushnumber(L, state);
			return 1;
		}
		lua_pushnumber(L, 2);
		return 1;
	}

	int WebSocket::lua_getOnOpen(lua_State* L){
		WebSocket* LuaWebSocket = checkWebSocket(L, 1);
		if(LuaWebSocket != NULL){
			return LuaWebSocket->OnOpen->wrap_lua(L);
		}
		lua_pushnil(L);
		return 1;
	}

	int WebSocket::lua_getOnClose(lua_State* L){
		WebSocket* LuaWebSocket = checkWebSocket(L, 1);
		if(LuaWebSocket != NULL){
			return LuaWebSocket->OnClose->wrap_lua(L);
		}
		lua_pushnil(L);
		return 1;
	}

	int WebSocket::lua_getOnMessage(lua_State* L){
		WebSocket* LuaWebSocket = checkWebSocket(L, 1);
		if(LuaWebSocket != NULL){
			return LuaWebSocket->OnMessage->wrap_lua(L);
		}
		lua_pushnil(L);
		return 1;
	}

	int WebSocket::lua_index(lua_State* L){
		WebSocket* LuaWebSocket = checkWebSocket(L, 1);
		if(LuaWebSocket != NULL){
			const char* name = luaL_checkstring(L, 2);

			lua_getmetatable(L, 1);
			lua_getfield(L, -1, "__properties");
			lua_getfield(L, -1, name);
			if(lua_iscfunction(L, -1)){
				lua_remove(L, -2);
				lua_remove(L, -2);

				lua_pushvalue(L, 1);
				lua_call(L, 1, 1);
				return 1;
			}else{
				lua_pop(L, 2);
				//Check methods
				lua_getfield(L, -1, "__methods");//-2
				lua_getfield(L, -1, name);//-1
				if(lua_iscfunction(L, -1)){
					lua_remove(L, -2);
					lua_remove(L, -3);

					return 1;
				}else{
					lua_pop(L, 3);
					return luaL_error(L, "attempt to index '%s' (a nil value)", name);
				}
			}
		}
		return 0;
	}

	int WebSocket::lua_newindex(lua_State* L){
		const char* name = luaL_checkstring(L, 2);
		return luaL_error(L, "%s cannot be assigned to", name);
	}

	int WebSocket::lua_toString(lua_State* L){
		WebSocket* LuaWebSocket = checkWebSocket(L, 1);
		std::string ret = "";
		ret += "WebSocket (" + std::string(LuaWebSocket->uri) + ")";
		lua_pushstring(L, ret.c_str());
		return 1;
	}

	int WebSocket::wrap_lua(lua_State* L){
		WebSocket** LuaWebSocket = (WebSocket**)lua_newuserdata(L, sizeof(*this));
		*LuaWebSocket = this;

		luaL_getmetatable(L, lua_websocket_name);
		lua_setmetatable(L, -2);
		return 1;
	}

	WebSocket* checkWebSocket(lua_State* L, int n){
		return *(WebSocket**)luaL_checkudata(L, n, lua_websocket_name);
	}
}
