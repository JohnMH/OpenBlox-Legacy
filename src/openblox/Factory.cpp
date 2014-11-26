#include "Factory.h"

namespace OpenBlox{
	void Factory::addClass(const char* className, ClassMaker* const newClassMaker){
		char* key = covertToLower(className);
		lokupTable[key] = newClassMaker;
	}

	void* Factory::create(const char* className){
		char* key = covertToLower(className);

		void* result = NULL;
		std::map<const char*, ClassMaker*>::iterator it = lokupTable.find(key);

		if(it != lokupTable.end())
			result = (it->second)->getInstance();
		return result;
	}

	void Factory::releaseTable(){
		for(std::map<const char*, ClassMaker*>::iterator it = lokupTable.begin(); it != lokupTable.end(); ++it){
			delete it->second;
		}
	}
}
