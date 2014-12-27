#include "LuaEvent.h"
#include "../openblox/ThreadScheduler.h"

#include "../ob_instance/Instance.h"

namespace ob_type{
	VarWrapper::VarWrapper(){
		type = TYPE_UNKNOWN;
		wrapped = NULL;
	}

	VarWrapper::VarWrapper(int var){
		type = TYPE_INT;
		wrapped = reinterpret_cast<void*>(var);
	}

	VarWrapper::VarWrapper(double var){
		type = TYPE_DOUBLE;
		wrapped = reinterpret_cast<void*>(&var);
	}

	VarWrapper::VarWrapper(bool var){
		type = TYPE_BOOL;
		wrapped = reinterpret_cast<void*>(var);
	}

	VarWrapper::VarWrapper(char* var){
		type = TYPE_CHAR;
		wrapped = reinterpret_cast<void*>(var);
	}

	VarWrapper::VarWrapper(const char* var){
		type = TYPE_CONST_CHAR;
		wrapped = reinterpret_cast<void*>(const_cast<char*>(var));
	}

	VarWrapper::VarWrapper(ob_instance::Instance* var){
		type = TYPE_INSTANCE;
		wrapped = reinterpret_cast<void*>(var);
	}

	VarWrapper::~VarWrapper(){}

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

	LuaEventConnection* checkEventConnection(lua_State* L, int n){
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
		LuaEventConnection* con = checkEventConnection(L, 1);
		con->disconnect();
		return 0;
	}

	int LuaEventConnection::lua_index(lua_State* L){
		LuaEventConnection* con = checkEventConnection(L, 1);
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
		LuaEventConnection* con = checkEventConnection(L, 1);
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

		connections = std::vector<EvtCon>();
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
			if(connections[i].ref == ref){
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
			if(connections[i].ref == ref){
				return true;
			}
		}
		return false;
	}

	LuaEvent* checkEvent(lua_State* L, int n){
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
		LuaEvent* LuaEvent = checkEvent(L, 1);
		std::string ret = "Signal ";
		ret += LuaEvent->LuaEventName;
		lua_pushstring(L, ret.c_str());
		return 1;
	}

	int LuaEvent::lua_connect(lua_State* L){
		LuaEvent* evt = checkEvent(L, 1);

		luaL_checktype(L, 2, LUA_TFUNCTION);
		lua_pushvalue(L, 2);

		int r = luaL_ref(L, LUA_REGISTRYINDEX);

		EvtCon econ = EvtCon();
		econ.env = L;
		econ.ref = r;

		evt->connections.push_back(econ);

		LuaEventConnection* con = new LuaEventConnection(evt, r);
		return con->wrap_lua(L);
	}

	int LuaEvent::lua_wait(lua_State* L){
		LuaEvent* evt = checkEvent(L, 1);
		evt->waiting.push_back(L);
		return lua_yield(L, evt->nargs);
	}

	void LuaEvent::pushWrappersToLua(lua_State* L, std::vector<VarWrapper> argList){
		for(size_t i = 0; i < argList.size(); i++){
			VarWrapper wrap = argList[i];
			switch(wrap.type){
				case TYPE_INT:
					lua_pushnumber(L, reinterpret_cast<int &>(wrap.wrapped));
					break;
				case TYPE_DOUBLE:
					lua_pushnumber(L, reinterpret_cast<double &>(wrap.wrapped));
					break;
				case TYPE_BOOL:
					lua_pushboolean(L, reinterpret_cast<bool &>(wrap.wrapped));
					break;
				case TYPE_CHAR:
					lua_pushstring(L, reinterpret_cast<char*>(wrap.wrapped));
					break;
				case TYPE_CONST_CHAR:
					lua_pushstring(L, reinterpret_cast<const char*>(wrap.wrapped));
					break;
				case TYPE_INSTANCE:
					reinterpret_cast<ob_instance::Instance*>(wrap.wrapped)->wrap_lua(L);
					break;
				default:
					lua_pushnil(L);
			}
		}
	}

	void LuaEvent::Fire(std::vector<VarWrapper> argList){
		if(!waiting.empty()){
			while(!waiting.empty()){
				try{
					lua_State* L = waiting.at(waiting.size() - 1);
					if(L != NULL){
						pushWrappersToLua(L, argList);

						OpenBlox::ThreadScheduler::AddWaitingTask(L, nargs);
					}
					waiting.pop_back();
				}catch(const std::out_of_range& e){}
			}
		}
		for(std::vector<int>::size_type i = 0; i != connections.size(); i++){
			EvtCon con = connections[i];
			lua_State* L = con.env;
			int ref = con.ref;

			OpenBlox::ThreadScheduler::AddWaitingEvent(L, ref, argList, nargs);
		}
	}
}
