#include "Thread.h"

namespace OpenBlox{
	std::vector<Thread*> Thread::allThreads = std::vector<Thread*>();

	Thread::Thread(thread_func* func){
		this->func = func;
	}

	Thread::~Thread(){
		std::vector<Thread*>::size_type min1 = -1;
		std::vector<Thread*>::size_type to_remove = min1;
		for(std::vector<Thread*>::size_type i = 0; i != allThreads.size(); i++){
			Thread* tkid = allThreads[i];
			if(tkid != NULL){
				if(tkid == this){
					to_remove = i;
					break;
				}
			}
		}
		if(to_remove != min1){
			allThreads.erase(allThreads.begin() + (to_remove - 1));
		}
	}

	int Thread::start(){
		allThreads.push_back(this);

		return pthread_create(&thread_struct, NULL, threadFunc, this);
	}

	void Thread::join(){
		void* status;

		pthread_join(thread_struct, &status);
	}

	bool Thread::isCurrent(){
		return this == getCurrent();
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

	Thread* Thread::getCurrent(){
		pthread_t theThread = pthread_self();

		for(std::vector<Thread*>::size_type i = 0; i != allThreads.size(); i++){
			Thread* tkid = allThreads[i];
			if(tkid != NULL){
				if(pthread_equal(theThread, tkid->thread_struct) != 0){
					return tkid;
				}
			}
		}

		return NULL;
	}
}
