#include "BaseGame.h"

#include "../ob_instance/DataModel.h"

namespace OpenBlox{
	static BaseGame* INSTANCE;

	lua_State* BaseGame::GlobalLuaState = NULL;
	Factory* BaseGame::InstanceFactory = NULL;

	BaseGame::BaseGame(){
		INSTANCE = this;
		GlobalLuaState = newLuaState();
		datamodel = new ob_instance::DataModel();
	}

	BaseGame::~BaseGame(){
		INSTANCE = NULL;
	}

	ob_instance::DataModel* BaseGame::getDataModel(){
		return datamodel;
	}

	//TODO: Implement LogService print, warn, error
	void BaseGame::print(const char* output){
		LOGI(output);
	}

	void BaseGame::warn(const char* output){
		LOGW(output);
	}

	void BaseGame::print_error(const char* output){
		LOGE(output);
	}

	void BaseGame::handle_lua_errors(lua_State* L){
		const char* output = lua_tostring(L, -1);
		std::string outstring = std::string(output);
		outstring = outstring + "\n";
		if(INSTANCE != NULL){
			INSTANCE->print_error(outstring.c_str());
		}
		lua_pop(L, 1);
	}

	static int lua_print(lua_State* L){
		std::string output = "";

		int n = lua_gettop(L);
		int i;

		lua_getglobal(L, "tostring");
		for(i=1; i <= n; i++){
			const char* s;
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

		output = output + "\n";

		if(INSTANCE != NULL){
			INSTANCE->print(output.c_str());
		}

		return 0;
	}

	static int lua_warn(lua_State* L){
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
			const char* s;
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

		output = output + "\n";

		if(INSTANCE != NULL){
			INSTANCE->warn(output.c_str());
		}

		return 0;
	}

	static int lua_newInstance(lua_State* L){
		const char* className = luaL_checkstring(L, 1);
		if(className != NULL){
			if(BaseGame::InstanceFactory != NULL){
				ob_instance::Instance* newGuy = BaseGame::InstanceFactory->create(className);
				if(newGuy != NULL){
					return newGuy->wrap_lua(L);
				}
			}
		}
		lua_pushnil(L);
		return 1;
	}

	lua_State* BaseGame::newLuaState(){
		lua_State* L = lua_open();
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

		lua_newtable(L);
		luaL_Reg instancelib[]{
			{"new", lua_newInstance},
			{NULL, NULL}
		};
		luaL_register(L, NULL, instancelib);
		lua_setglobal(L, "Instance");

		lua_register(L, "print", lua_print);
		lua_register(L, "warn", lua_warn);

		return L;
	}

	lua_State* BaseGame::getGlobalState(){
		return GlobalLuaState;
	}

	Factory* BaseGame::getInstanceFactory(){
		return InstanceFactory;
	}

	BaseGame* BaseGame::getInstance(){
		return INSTANCE;
	}
}
