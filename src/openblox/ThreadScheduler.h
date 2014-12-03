#ifndef OPENBLOX_THREADSCHEDULER_H_
#define OPENBLOX_THREADSCHEDULER_H_

#include "OpenBlox.h"

namespace OpenBlox{
	class ThreadScheduler{
		public:
			struct Task{
				lua_State* origin;
				int coro;
				bool usestackid;
				long at;
				long start;
			};

			ThreadScheduler();
			virtual ~ThreadScheduler();

			static int Delay(lua_State* L, int funcidx, long millis);
			static int Spawn(lua_State* L, int funcidx);
			static int Wait(lua_State* L, long millis);

			static void Tick();
		private:
			static void enqueue_task(Task t);
	};
}
#endif
