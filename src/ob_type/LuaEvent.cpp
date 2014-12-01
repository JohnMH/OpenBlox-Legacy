#include "LuaEvent.h"

namespace ob_type {

	STATIC_INIT(LuaEvent){
		lua_State* L = OpenBlox::BaseGame::getGlobalState();
		lua_newtable(L);
		lua_setfield(L, LUA_REGISTRYINDEX, "EventThreads");

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
				{NULL, NULL}
		};
		luaL_register(L, NULL, methods);

		lua_rawset(L, -3);

		lua_pop(L, 1);
	}

	LuaEvent::LuaEvent(const char* LuaEventName) {
		this->LuaEventName = LuaEventName;
		Connections = std::vector<lua_State*>();
	}

	LuaEvent::~LuaEvent() {
		//free(LuaEventName);
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
		LuaEvent* LuaEvent = checkudata(L, 1);
		luaL_checktype(L, 2, LUA_TFUNCTION);

		lua_State* func = lua_newthread(L);
		lua_pushvalue(L, 1);
		lua_xmove(L, func, 1);
		LuaEvent->Connections.push_back(func);
	}

	void LuaEvent::Fire(){
		std::cout<<"Fire called!"<<std::endl;
		for (std::vector<lua_State*>::size_type i = 0; i != Connections.size(); i++){
			lua_State* thread = Connections[i];
			if (thread != NULL){
				std::cout<<"Resuming a thread!"<<std::endl;
				lua_resume(thread, 0);
			}
		}
	}

} /* namespace ob_type */
