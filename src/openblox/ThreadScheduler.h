#ifndef OPENBLOX_THREADSCHEDULER_H_
#define OPENBLOX_THREADSCHEDULER_H_

#include "OpenBlox.h"

namespace OpenBlox{
	typedef void (task_func)();

	class ThreadScheduler{
		public:
			struct Task{
				lua_State* origin;
				int ref = -1;
				long at;
				long start;
			};
			struct WaitingTask{
				lua_State* state;
				int resumeArgs;
			};
			struct WaitingFuncTask{
				task_func* func;
				long at;
			};
			struct less_than_key{
				inline bool operator() (const Task& struct1, const Task& struct2){
					return struct1.at > struct2.at;
				}
			};
			struct func_less_than_key{
				inline bool operator() (const WaitingFuncTask& struct1, const WaitingFuncTask& struct2){
					return struct1.at > struct2.at;
				}
			};

			static int Delay(lua_State* L, int funcidx, long millis);
			static int Spawn(lua_State* L, int funcidx);
			static int Wait(lua_State* L, long millis);
			static void AddWaitingTask(lua_State* L, int nargs);
			static void RunOnTaskThread(task_func* func, long millis);

			static void Tick();
		private:
			static void enqueue_task(Task t);
	};
}
#endif
