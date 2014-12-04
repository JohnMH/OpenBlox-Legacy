#include "ThreadScheduler.h"

namespace OpenBlox{
	ThreadScheduler::ThreadScheduler(){}
	ThreadScheduler::~ThreadScheduler(){}

	static std::vector<ThreadScheduler::Task> tasks = std::vector<ThreadScheduler::Task>();

	int ThreadScheduler::Delay(lua_State* L, int funcidx, long millis){
		lua_State* NL = lua_newthread(L);
		lua_pushvalue(L, funcidx);
		lua_xmove(L, NL, 1);
		int r = luaL_ref(L, LUA_REGISTRYINDEX);

		long curTime = currentTimeMillis();

		Task tsk = Task();
		tsk.origin = NL;
		tsk.at = curTime + millis;
		tsk.start = curTime;
		tsk.ref = r;

		enqueue_task(tsk);

		return 0;
	}

	int ThreadScheduler::Spawn(lua_State* L, int funcidx){
		return Delay(L, funcidx, 0);
	}

	int ThreadScheduler::Wait(lua_State* L, long millis){
		long curTime = currentTimeMillis();

		Task tsk = Task();
		tsk.origin = L;
		tsk.at = curTime + millis;
		tsk.start = curTime;

		enqueue_task(tsk);

		return lua_yield(L, 2);
	}

	void ThreadScheduler::enqueue_task(ThreadScheduler::Task t){
		tasks.push_back(t);
		std::sort(tasks.begin(), tasks.end(), less_than_key());
	}

	void ThreadScheduler::Tick(){
		if(!tasks.empty()){
			long curTime = currentTimeMillis();
			try{
				Task task = tasks.at(tasks.size() - 1);
				if(task.at < curTime){
					lua_State* L = task.origin;
					if(L == NULL){
						return;
					}

					long appStartTime = OpenBlox::BaseGame::appStarted();
					long elapsedTime = (curTime - task.start) / 1000;

					lua_pushnumber(L, elapsedTime);
					lua_pushnumber(L, (curTime - appStartTime) / 1000.0);

					luaL_unref(L, LUA_REGISTRYINDEX, task.ref);
					tasks.pop_back();
					int stat = lua_resume(L, 2);
					if(stat != 0 && stat != LUA_YIELD){
						OpenBlox::BaseGame::getInstance()->handle_lua_errors(L);
					}
				}
			}catch(const std::out_of_range& e){}
		}
	}
}
