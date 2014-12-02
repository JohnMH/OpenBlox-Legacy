#ifndef OPENBLOX_THREADSCHEDULER_H_
#define OPENBLOX_THREADSCHEDULER_H_

#include "OpenBlox.h"

namespace OpenBlox {
	class ThreadScheduler {
		public:
			struct Task {
				lua_State* origin; // original lua_State
				int coro; // stack id from registry
				bool usestackid; // use stack id?
				long at; // when to resume
			};

			ThreadScheduler();
			virtual ~ThreadScheduler();

			static void Delay(lua_State* L, int funcidx, long millis);
			static void Spawn(lua_State* L, int funcidx);
			static void Wait(lua_State* L, long millis);

			static void Tick();
		private:
			static void enqueue_task(Task t);
	};

} /* namespace OpenBlox */

#endif /* OPENBLOX_THREADSCHEDULER_H_ */
