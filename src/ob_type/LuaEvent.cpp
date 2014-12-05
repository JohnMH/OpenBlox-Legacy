#include "LuaEvent.h"
#include "../openblox/ThreadScheduler.h"

namespace ob_type{
	STATIC_INIT(LuaEventConnection){
		lua_State* L = OpenBlox::BaseGame::getGlobalState();

		luaL_newmetatable(L, lua_evt_con_name);

		lua_pushstring(L, "__metatable");
		lua_pushstring(L, "This metatable is locked");
		lua_rawset(L, -3);

		lua_pushstring(L, "__methods");
		lua_newtable(L);
		luaL_Reg methods[]{
			{"disconnect", lua_disconnect},
			{NULL, NULL}
		};
		luaL_register(L, NULL, methods);
		lua_rawset(L, -3);

		lua_pushstring(L, "__properties");
		lua_newtable(L);
		luaL_Reg props[]{
			{"connected", lua_getConnected},
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

	LuaEventConnection::LuaEventConnection(LuaEvent* evt, int ref){
		this->evt = evt;
		this->ref = ref;
		connected = true;
	}

	LuaEventConnection::~LuaEventConnection(){}

	void LuaEventConnection::disconnect(){
		if(connected){
			evt->disconnect(ref);
			connected = false;
		}
	}

	LuaEventConnection* checkconn(lua_State *L, int n){
		return *(LuaEventConnection**)luaL_checkudata(L, n, lua_evt_con_name);
	}

	int LuaEventConnection::wrap_lua(lua_State* L){
		LuaEventConnection** evt = (LuaEventConnection**)lua_newuserdata(L, sizeof(*this));
		*evt = this;

		luaL_getmetatable(L, lua_evt_con_name);
		lua_setmetatable(L, -2);

		return 1;
	}

	int LuaEventConnection::lua_toString(lua_State* L){
		lua_pushstring(L, "Connection");
		return 1;
	}

	int LuaEventConnection::lua_disconnect(lua_State* L){
		LuaEventConnection* con = checkconn(L, 1);
		con->disconnect();
		return 0;
	}

	int LuaEventConnection::lua_index(lua_State* L){
		LuaEventConnection* con = checkconn(L, 1);
		if(con != NULL){
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

	int LuaEventConnection::lua_newindex(lua_State* L){
		const char* name = luaL_checkstring(L, 2);
		return luaL_error(L, "%s cannot be assigned to", name);
	}

	int LuaEventConnection::lua_getConnected(lua_State* L){
		LuaEventConnection* con = checkconn(L, 1);
		if(con != NULL){
			bool conned = con->connected;
			if(conned){
				conned = con->evt->isConnected(con->ref);
			}
			lua_pushboolean(L, conned);
			return 1;
		}
		return 0;
	}

	STATIC_INIT(LuaEvent){
		lua_State* L = OpenBlox::BaseGame::getGlobalState();
		//lua_newtable(L);
		//lua_setfield(L, LUA_REGISTRYINDEX, "EventThreads");

		luaL_newmetatable(L, lua_evt_name);

		lua_pushstring(L, "__metatable");
		lua_pushstring(L, "This metatable is locked");
		lua_rawset(L, -3);

		lua_pushstring(L, "__tostring");
		lua_pushcfunction(L, lua_toString);
		lua_rawset(L, -3);

		lua_pushstring(L, "__index");
		lua_newtable(L);

		luaL_Reg methods[]{
			{"connect", lua_connect},
			{"wait", lua_wait},
			{NULL, NULL}
		};
		luaL_register(L, NULL, methods);

		lua_rawset(L, -3);

		lua_pop(L, 1);
	}

	LuaEvent::LuaEvent(const char* LuaEventName, int nargs){
		this->LuaEventName = LuaEventName;
		this->nargs = nargs;

		connections = std::vector<int>();
		waiting = std::vector<lua_State*>();
	}

	LuaEvent::~LuaEvent(){}

	void LuaEvent::disconnectAll(){
		connections.clear();
	}

	void LuaEvent::disconnect(int ref){
		std::vector<int>::size_type min1 = -1;
		std::vector<int>::size_type to_remove = min1;
		for(std::vector<int>::size_type i = 0; i != connections.size(); i++){
			if(connections[i] == ref){
				to_remove = i;
				break;
			}
		}
		if(to_remove != min1){
			connections.erase(connections.begin() + (to_remove - 1));
		}
	}

	bool LuaEvent::isConnected(int ref){
		for(std::vector<int>::size_type i = 0; i != connections.size(); i++){
			if(connections[i] == ref){
				return true;
			}
		}
		return false;
	}

	LuaEvent* checkudata(lua_State *L, int n){
		return *(LuaEvent**)luaL_checkudata(L, n, lua_evt_name);
	}

	int LuaEvent::wrap_lua(lua_State* L){
		LuaEvent** evt = (LuaEvent**)lua_newuserdata(L, sizeof(*this));
		*evt = this;

		luaL_getmetatable(L, lua_evt_name);
		lua_setmetatable(L, -2);

		return 1;
	}

	int LuaEvent::lua_toString(lua_State* L){
		LuaEvent* LuaEvent = checkudata(L, 1);
		std::string ret = "Signal ";
		ret += LuaEvent->LuaEventName;
		lua_pushstring(L, ret.c_str());
		return 1;
	}

	int LuaEvent::lua_connect(lua_State* L){
		LuaEvent* evt = checkudata(L, 1);
		luaL_checktype(L, 2, LUA_TFUNCTION);

		lua_pushvalue(L, 2);

		int r = luaL_ref(L, LUA_REGISTRYINDEX);
		evt->connections.push_back(r);

		LuaEventConnection* con = new LuaEventConnection(evt, r);
		return con->wrap_lua(L);
	}

	int LuaEvent::lua_wait(lua_State* L){
		LuaEvent* evt = checkudata(L, 1);
		evt->waiting.push_back(L);
		return lua_yield(L, evt->nargs);
	}

	void LuaEvent::Fire(luaFireFunc fireFunc, ...){
		lua_State* L = OpenBlox::BaseGame::getGlobalState();
		if(!waiting.empty()){
			while(!waiting.empty()){
				try{
					lua_State* L = waiting.at(waiting.size() - 1);
					if(L != NULL){
						va_list ap;
						va_start(ap, fireFunc);

						fireFunc(L, ap);

						va_end(ap);

						OpenBlox::ThreadScheduler::AddWaitingTask(L, nargs);
					}
					waiting.pop_back();
				}catch(const std::out_of_range& e){}
			}
		}
		for(std::vector<int>::size_type i = 0; i != connections.size(); i++){
			int ref = connections[i];

			lua_State* eL = lua_newthread(L);
			lua_resume(eL, 0);

			lua_rawgeti(eL, LUA_REGISTRYINDEX, ref);

			va_list ap;
			va_start(ap, fireFunc);

			fireFunc(eL, ap);

			va_end(ap);

			int s = lua_pcall(eL, nargs, 0, 0);
			if(s != 0){
				OpenBlox::BaseGame::getInstance()->handle_lua_errors(eL);
			}
		}
	}
}
