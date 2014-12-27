#ifndef OPENBLOX_THREADSCHEDULER_H_
#define OPENBLOX_THREADSCHEDULER_H_

#include "OpenBlox.h"

namespace ob_type{
	class VarWrapper;
}

namespace OpenBlox{
	typedef void (task_func)(va_list);
	typedef void (*luaFireFunc)(lua_State*, va_list);

	class ThreadScheduler{
		public:
			struct Task{
				lua_State* origin;
				int ref;
				long at;
				long start;
			};
			struct WaitingTask{
				lua_State* state;
				int resumeArgs;
				bool isCall;
			};
			struct WaitingFuncTask{
				task_func* func;
				long at;
				va_list args;
			};
			struct WaitingEvent{
				lua_State* state;
				int ref;
				std::vector<ob_type::VarWrapper> argList;
				int numArgs;
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
			static void AddWaitingCall(lua_State* L, int nargs);
			static void AddWaitingEvent(lua_State* L, int ref, std::vector<ob_type::VarWrapper> argList, int numArgs);
			static void RunOnTaskThread(task_func* func, long millis, ...);
			static bool isOnTaskThread();

			static void Tick();

			static Thread* taskThread;
		private:
			static void enqueue_task(Task t);
	};
}
#endif
