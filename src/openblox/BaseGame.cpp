#include "BaseGame.h"

namespace OpenBlox{
	static BaseGame *INSTANCE;

	lua_State *BaseGame::GlobalLuaState = NULL;
	Factory *BaseGame::InstanceFactory = NULL;

	BaseGame::BaseGame(){
		INSTANCE = this;
		GlobalLuaState = newLuaState();
	}

	BaseGame::~BaseGame(){
		INSTANCE = NULL;
	}

	//TODO: Implement LogService print, warn, error
	int BaseGame::print(lua_State *L){
			std::string output = "";

			int n = lua_gettop(L);
			int i;

			lua_getglobal(L, "tostring");
			for(i=1; i <= n; i++){
				const char *s;
				lua_pushvalue(L, -1);
				lua_pushvalue(L, i);
				lua_call(L, 1, 1);
				s = lua_tostring(L, -1);
				if(s == NULL){
					return luaL_error(L, LUA_QL("tostring") " must return a string to " LUA_QL("print"));
				}
				if(i > 1){
					output = output + "\t";
				}
				output = output + s;
				lua_pop(L, 1);
			}

			std::cout << output << std::endl;

			return 0;
		}

	int BaseGame::warn(lua_State *L){
		std::string output = "";

		int n = lua_gettop(L);
		int i;

		lua_Debug ar;
		if(lua_getstack(L, 1, &ar)){
			lua_getinfo(L, "Sl", &ar);
			if(ar.currentline > 0){
				output = output + ar.short_src + ":" + ((std::ostringstream&)(std::ostringstream() << std::dec << ar.currentline)).str() + ": ";
			}
		}

		lua_getglobal(L, "tostring");
		for(i=1; i <= n; i++){
			const char *s;
			lua_pushvalue(L, -1);
			lua_pushvalue(L, i);
			lua_call(L, 1, 1);
			s = lua_tostring(L, -1);
			if(s == NULL){
				return luaL_error(L, LUA_QL("tostring") " must return a string to " LUA_QL("print"));
			}
			if(i > 1){
				output = output + "\t";
			}
			output = output + s;
			lua_pop(L, 1);
		}

		std::cout << output << std::endl;

		return 0;
	}

	void BaseGame::handle_errors(lua_State *L){
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}

	static int print(lua_State *L){
		if(INSTANCE != NULL){
			return INSTANCE->print(L);
		}
		return 0;
	}

	static int warn(lua_State *L){
		if(INSTANCE != NULL){
			return INSTANCE->warn(L);
		}
		return 0;
	}

	lua_State* BaseGame::newLuaState(){
		lua_State *L = lua_open();
		luaopen_base(L);
		luaopen_table(L);
		luaopen_string(L);
		luaopen_math(L);

		lua_pushnil(L);
		lua_setglobal(L, "dofile");
		lua_pushnil(L);
		lua_setglobal(L, "load");
		lua_pushnil(L);
		lua_setglobal(L, "loadfile");

		lua_register(L, "print", OpenBlox::print);
		lua_register(L, "warn", OpenBlox::warn);

		return L;
	}

	lua_State* BaseGame::getGlobalState(){
		return GlobalLuaState;
	}

	Factory* BaseGame::getInstanceFactory(){
		return InstanceFactory;
	}
}
