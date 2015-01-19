#include "Thread.h"

namespace OpenBlox{
	std::vector<Thread*> Thread::allThreads = std::vector<Thread*>();

	Thread::Thread(thread_func* func){
		this->func = func;
		curThread = NULL;
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

	void Thread::start(){
		allThreads.push_back(this);

		curThread = new boost::thread(func);
	}

	void Thread::join(){
		if(curThread){
			curThread->join();
		}
	}

	bool Thread::isCurrent(){
		return this == getCurrent();
	}

	Thread* Thread::getCurrent(){
		boost::thread::id tID = boost::this_thread::get_id();

		for(std::vector<Thread*>::size_type i = 0; i != allThreads.size(); i++){
			Thread* tkid = allThreads[i];
			if(tkid != NULL){
				if(boost::thread* tT = tkid->curThread){
					if(tT->get_id() == tID){
						return tkid;
					}
				}
			}
		}

		return NULL;
	}
}
