#ifndef OPENBLOX_THREAD_H_
#define OPENBLOX_THREAD_H_

#include <pthread.h>

namespace OpenBlox{
	typedef void (thread_func)();

	class Thread{
		public:
			Thread(thread_func* func);
			virtual ~Thread();

			int start();
			void join();
		private:
			pthread_t thread_struct;
			thread_func* func;

			static void* threadFunc(void* myThread);
	};
};
#endif
