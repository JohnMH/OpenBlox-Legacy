#include "ThreadScheduler.h"

namespace OpenBlox{
	ThreadScheduler::ThreadScheduler(){}
	ThreadScheduler::~ThreadScheduler(){}

	static std::vector<ThreadScheduler::Task> tasks = std::vector<ThreadScheduler::Task>();

	int ThreadScheduler::Delay(lua_State* L, int funcidx, long millis){
		lua_State* NL = lua_newthread(L);
		luaL_argcheck(L, lua_isfunction(L, funcidx) && !lua_iscfunction(L, funcidx), funcidx, "Lua function expected");
		lua_pushvalue(L, funcidx);
		lua_xmove(L, NL, funcidx);

		long curTime = currentTimeMillis();

		Task tsk = Task();
		tsk.origin = L;
		tsk.at = curTime + millis;
		tsk.start = curTime;
		tsk.coro = lua_ref(L, LUA_REGISTRYINDEX);
		tsk.usestackid = true;

		enqueue_task(tsk);

		return 0;
	}

	int ThreadScheduler::Spawn(lua_State* L, int funcidx){
		lua_State* NL = lua_newthread(L);
		luaL_argcheck(L, lua_isfunction(L, funcidx) && !lua_iscfunction(L, funcidx), funcidx, "Lua function expected");
		lua_pushvalue(L, funcidx);
		lua_xmove(L, NL, funcidx);

		Task tsk = Task();
		tsk.origin = L;
		tsk.at = currentTimeMillis();
		tsk.start = tsk.at;
		tsk.coro = lua_ref(L, LUA_REGISTRYINDEX);
		tsk.usestackid = true;

		enqueue_task(tsk);

		return 0;
	}

	int ThreadScheduler::Wait(lua_State* L, long millis){
		long curTime = currentTimeMillis();

		Task tsk = Task();
		tsk.origin = L;
		tsk.at = curTime + millis;
		tsk.start = curTime;
		tsk.usestackid = false;

		enqueue_task(tsk);

		return lua_yield(L, 2);
	}

	void ThreadScheduler::enqueue_task(ThreadScheduler::Task t){
		tasks.push_back(t);
	}

	void ThreadScheduler::Tick(){
		if(!tasks.empty()){
			long curTime = currentTimeMillis();
			try{
				Task task = tasks.at(tasks.size() - 1);
				if(task.at < curTime){
					lua_State* L = NULL;
					if(task.usestackid == true){
						lua_rawgeti(task.origin, LUA_REGISTRYINDEX, task.coro);
						L = lua_tothread(task.origin, -1);
						luaL_unref(task.origin, LUA_REGISTRYINDEX, task.coro);
					}else{
						L = task.origin;
					}

					if(L == NULL){
						return;
					}

					long appStartTime = OpenBlox::BaseGame::appStarted();

					long elapsedTime = (curTime - task.start) / 1000;

					lua_pushnumber(L, 1);
					lua_pushnumber(L, (curTime - appStartTime) / 1000.0);
					lua_resume(L, 2);
					tasks.pop_back();
				}
			}catch(const std::out_of_range& e){}
		}
	}
}
