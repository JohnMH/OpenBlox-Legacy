#include "Thread.h"

namespace OpenBlox{
	Thread::Thread(thread_func* func){
		this->func = func;
	}

	Thread::~Thread(){}

	int Thread::start(){
		return pthread_create(&thread_struct, NULL, threadFunc, this);
	}

	void Thread::join(){
		void* status;

		pthread_join(thread_struct, &status);
	}

	void* Thread::threadFunc(void* myThread){
		Thread* theThread = (Thread*)myThread;
		if(theThread){
			if(theThread->func){
				theThread->func();
			}
		}
		pthread_exit(NULL);
		return NULL;
	}
}
