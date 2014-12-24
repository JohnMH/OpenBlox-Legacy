#include "BaseGame.h"

#include "../ob_instance/DataModel.h"

#include "../ob_type/Vector3.h"
#include "../ob_type/Vector2.h"

#include <ctime>

namespace OpenBlox{
	static BaseGame* INSTANCE;
	static long APP_START = currentTimeMillis();

	lua_State* BaseGame::GlobalLuaState = NULL;
	Factory* BaseGame::InstanceFactory = NULL;

	BaseGame::BaseGame(){
		INSTANCE = this;
		datamodel = new ob_instance::DataModel();

		GlobalLuaState = lua_open();
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
		if(INSTANCE != NULL){
			INSTANCE->print_error(output);
		}
		lua_pop(L, 1);
	}

	lua_State* BaseGame::newLuaState(){
		lua_State* L = lua_newthread(GlobalLuaState);

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

		lua_register(L, "delay", lua_delay);
		lua_register(L, "Delay", lua_delay);
		lua_register(L, "elapsedTime", lua_elapsedTime);
		lua_register(L, "ElapsedTime", lua_elapsedTime);
		lua_register(L, "spawn", lua_spawn);
		lua_register(L, "Spawn", lua_spawn);
		lua_register(L, "print", lua_print);
		lua_register(L, "warn", lua_warn);
		lua_register(L, "tick", lua_tick);
		lua_register(L, "wait", lua_wait);
		lua_register(L, "Wait", lua_wait);

		lua_newtable(L);
		luaL_Reg instancelib[]{
			{"new", lua_newInstance},
			{NULL, NULL}
		};
		luaL_register(L, NULL, instancelib);
		lua_setglobal(L, "Instance");

		lua_newtable(L);
		luaL_Reg vector3lib[]{
			{"new", lua_newVector3},
			{NULL, NULL}
		};
		luaL_register(L, NULL, vector3lib);
		lua_setglobal(L, "Vector3");

		lua_newtable(L);
		luaL_Reg vector2lib[]{
			{"new", lua_newVector2},
			{NULL, NULL}
		};
		luaL_register(L, NULL, vector2lib);
		lua_setglobal(L, "Vector2");

		ob_instance::DataModel* dm = INSTANCE->getDataModel();
		int gm = dm->wrap_lua(L);
		lua_pushvalue(L, -gm);
		lua_setglobal(L, "game");

		lua_pushvalue(L, -gm);
		lua_setglobal(L, "Game");

		lua_pop(L, gm);

		//lua_pop(GlobalLuaState, 1);

		return L;
	}

	lua_State* BaseGame::getGlobalState(){
		return GlobalLuaState;
	}

	Factory* BaseGame::getInstanceFactory(){
		return InstanceFactory;
	}

	long BaseGame::appStarted(){
		return APP_START;
	}

	BaseGame* BaseGame::getInstance(){
		return INSTANCE;
	}

	int BaseGame::lua_wait(lua_State* L){
		double waitTime = 1/60;
		if(!lua_isnoneornil(L, 1)){
			waitTime = luaL_checknumber(L, 1);
		}
		return ThreadScheduler::Wait(L, (waitTime * 1000));
	}

	int BaseGame::lua_delay(lua_State* L){
		if(lua_isnumber(L, 1)){
			double delayTime = luaL_checknumber(L, 1);

			luaL_checktype(L, 2, LUA_TFUNCTION);

			return ThreadScheduler::Delay(L, 2, delayTime * 1000);
		}else{
			luaL_checktype(L, 1, LUA_TFUNCTION);

			double delayTime = luaL_checknumber(L, 2);

			return ThreadScheduler::Delay(L, 1, delayTime * 1000);
		}
	}

	int BaseGame::lua_spawn(lua_State* L){
		luaL_checktype(L, 1, LUA_TFUNCTION);
		return ThreadScheduler::Spawn(L, 1);
	}

	int BaseGame::lua_tick(lua_State* L){
		lua_pushnumber(L, time(0));
		return 1;
	}

	int BaseGame::lua_print(lua_State* L){
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
			lua_pop(L, 1);
			if(s == NULL){
				return luaL_error(L, LUA_QL("tostring") " must return a string to " LUA_QL("print"));
			}
			if(i > 1){
				output = output + "\t";
			}
			output = output + s;
		}

		if(INSTANCE != NULL){
			INSTANCE->print(output.c_str());
		}

		return 0;
	}

	int BaseGame::lua_elapsedTime(lua_State* L){
		lua_pushnumber(L, (currentTimeMillis() - APP_START) / 1000.0);
		return 1;
	}

	int BaseGame::lua_warn(lua_State* L){
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

		if(INSTANCE != NULL){
			INSTANCE->warn(output.c_str());
		}

		return 0;
	}

	int BaseGame::lua_newInstance(lua_State* L){
		const char* className = luaL_checkstring(L, 1);
		ob_instance::Instance* par = ob_instance::Instance::checkInstance(L, 2);
		if(className != NULL){
			if(BaseGame::InstanceFactory != NULL){
				ob_instance::Instance* newGuy = BaseGame::InstanceFactory->create(className);
				if(newGuy != NULL){
					if(par != NULL){
						try{
							newGuy->setParent(par);
						}catch(std::runtime_error& ex){
							return luaL_error(L, ex.what());
						}
					}
					return newGuy->wrap_lua(L);
				}
			}
		}
		lua_pushnil(L);
		return 1;
	}

	int BaseGame::lua_newVector3(lua_State* L){
		double x = luaL_checknumber(L, 1);
		double y = luaL_checknumber(L, 2);
		double z = luaL_checknumber(L, 3);

		ob_type::Vector3* newGuy = new ob_type::Vector3(x, y, z);
		return newGuy->wrap_lua(L);
	}

	int BaseGame::lua_newVector2(lua_State* L){
		double x = luaL_checknumber(L, 1);
		double y = luaL_checknumber(L, 2);

		ob_type::Vector2* newGuy = new ob_type::Vector2(x, y);
		return newGuy->wrap_lua(L);
	}
}
