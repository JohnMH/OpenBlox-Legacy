#include "BaseGame.h"

#include "../ob_instance/DataModel.h"

namespace OpenBlox{
	static BaseGame* INSTANCE;

	lua_State* BaseGame::GlobalLuaState = NULL;
	Factory* BaseGame::InstanceFactory = NULL;

	/*
	struct WaitingTask{
		WaitingTask(lua_State* L, long resumeTime){
			this->L = L;
			this->resumeTime = resumeTime;
		}

		lua_State* L;
		long resumeTime;
	};

	static std::vector<WaitingTask*> tasks = std::vector<WaitingTask*>();
	*/

	BaseGame::BaseGame(){
		INSTANCE = this;

		lua_State* L = lua_open();

		GlobalLuaState = L;
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
		if(INSTANCE != NULL){
			INSTANCE->print_error(output);
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

	/*
	static int lua_wait(lua_State* L){
		double waitTime = 1/60;
		if(!lua_isnoneornil(L, 1)){
			waitTime = luaL_checknumber(L, 1);
		}

		long waitStart = currentTimeMillis();
		long longWaitTime = waitStart + (waitTime * 1000);

		tasks.push_back(new WaitingTask(L, longWaitTime));

		return lua_yield(L, 2);
	}
	*/

	lua_State* BaseGame::newLuaState(){
		lua_State* L = lua_newthread(GlobalLuaState);
		lua_resume(L, 0);

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

		//lua_register(L, "wait", lua_wait);

		ob_instance::DataModel* dm = INSTANCE->getDataModel();
		int gm = dm->wrap_lua(L);
		lua_pushvalue(L, -gm);
		lua_setglobal(L, "game");

		lua_pushvalue(L, -gm);
		lua_setglobal(L, "Game");

		lua_pop(L, gm);

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

	/*
	void removeTask(WaitingTask* task){
		std::vector<WaitingTask*>::size_type min1 = -1;
		std::vector<WaitingTask*>::size_type to_remove = min1;
		for(std::vector<WaitingTask*>::size_type i = 0; i != tasks.size(); i++){
			if(tasks[i] == task){
				to_remove = i;
				break;
			}
		}
		if(to_remove != min1){
			tasks.erase(tasks.begin() + (to_remove - 1));
		}
	}

	void BaseGame::tick(){
		if(tasks.size() > 0){
			long curTime = OpenBlox::currentTimeMillis();

			for(std::vector<WaitingTask*>::size_type i = 0; tasks.size(); i++){
				WaitingTask* task = tasks[i];
				if(task != NULL){
					if(lua_status(task->L) == LUA_YIELD){
						if(task->resumeTime <= curTime){
							lua_pushnumber(task->L, curTime - task->resumeTime);
							lua_pushnumber(task->L, curTime);
							int s = lua_resume(task->L, 2);
							if(s != 0 && s != LUA_YIELD){
								INSTANCE->handle_lua_errors(task->L);
							}
							removeTask(task);
						}
					}
				}else{
					removeTask(task);
				}
			}
		}
	}
	*/
}
