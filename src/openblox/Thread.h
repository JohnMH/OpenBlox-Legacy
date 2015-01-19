#ifndef OPENBLOX_THREAD_H_
#define OPENBLOX_THREAD_H_

#include <boost/thread.hpp>

#include <vector>

#include <cstdio>

namespace OpenBlox{
	typedef void (thread_func)();

	class Thread{
		public:
			Thread(thread_func* func);
			virtual ~Thread();

			void start();
			void join();
			bool isCurrent();

			static Thread* getCurrent();
		private:
			boost::thread* curThread;
			thread_func* func;

			static std::vector<Thread*> allThreads;
	};
};
#endif
