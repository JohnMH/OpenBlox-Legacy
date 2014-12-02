#include "ThreadScheduler.h"

namespace OpenBlox {

	ThreadScheduler::ThreadScheduler() {
	}

	ThreadScheduler::~ThreadScheduler() {
	}

	static std::vector<ThreadScheduler::Task> tasks = std::vector<ThreadScheduler::Task>();

	void ThreadScheduler::Delay(lua_State* L, int funcidx, long millis){
		lua_State* NL = lua_newthread(L);
		luaL_argcheck(L, lua_isfunction(L, funcidx) && !lua_iscfunction(L, funcidx), funcidx, "Lua function expected");
		lua_pushvalue(L, funcidx);
		lua_xmove(L, NL, funcidx);

		Task tsk = Task();
		tsk.origin = L;
		tsk.at = currentTimeMillis() + millis;
		tsk.coro = lua_ref(L, LUA_REGISTRYINDEX);
		tsk.usestackid = true;

		enqueue_task(tsk);
	}
	void ThreadScheduler::Spawn(lua_State* L, int funcidx){
		lua_State* NL = lua_newthread(L);
		luaL_argcheck(L, lua_isfunction(L, funcidx) && !lua_iscfunction(L, funcidx), funcidx, "Lua function expected");
		lua_pushvalue(L, funcidx);
		lua_xmove(L, NL, funcidx);

		Task tsk = Task();
		tsk.origin = L;
		tsk.at = currentTimeMillis();
		tsk.coro = lua_ref(L, LUA_REGISTRYINDEX);
		tsk.usestackid = true;

		enqueue_task(tsk);
	}

	void ThreadScheduler::Wait(lua_State* L, long millis){
		Task tsk =Task();
		tsk.origin = L;
		tsk.at = currentTimeMillis() + millis;
		tsk.usestackid = false;

		LOGI("Queueing");
		enqueue_task(tsk);
	}

	void ThreadScheduler::enqueue_task(ThreadScheduler::Task t){
		//lua_State* L = BaseGame::getGlobalState();
		tasks.push_back(t);
	}

	void ThreadScheduler::Tick(){
		if (!tasks.empty()){
			//lua_State* M = BaseGame::getGlobalState();
			long curTime = currentTimeMillis();
			try {
				Task task = tasks.at(tasks.size()-1);
				if (task.at < curTime){
					lua_State* L = NULL;
					if (task.usestackid == true){
						LOGI("new thread");
						lua_rawgeti(task.origin, LUA_REGISTRYINDEX, task.coro);
						L = lua_tothread(task.origin, -1);
						luaL_unref(task.origin, LUA_REGISTRYINDEX, task.coro);
					} else {
						LOGI("yield");
						L = task.origin;
					}

					LOGI("Status: %d %d", lua_status(L), LUA_YIELD);
					LOGI("top: %d", lua_gettop(L));
					for (int i = 0; i<=lua_gettop(L); i++){
						LOGI("type of %d: %s", i, lua_typename(L, lua_type(L, i)));
					}

					//LOGI("push0");
					//lua_pushvalue(L, lua_gettop(L)-1);
					//LOGI("push1");
					//lua_pushnumber(L, curTime - task.at);
					//LOGI("push2");
					//lua_pushnumber(L, curTime);
					LOGI("lua_resume");
					lua_resume(L, 2);
					LOGI("Pop");
					tasks.pop_back();
				}
			} catch(const std::out_of_range& e) {}
		}
	}

} /* namespace OpenBlox */
