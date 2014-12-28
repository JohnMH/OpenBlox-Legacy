#include "Factory.h"

#include "../ob_instance/Instance.h"

namespace OpenBlox{
	void Factory::addClass(std::string className, ClassMaker* const newClassMaker){
		lokupTable[className] = newClassMaker;
		registered.push_back(className);
	}

	ob_instance::Instance* Factory::create(std::string className){
		if(lokupTable[className]){
			ClassMaker* maker = lokupTable[className];
			if(maker->isInstantiatable()){
				return maker->getInstance();
			}
		}

		return NULL;
	}

	ob_instance::Instance* Factory::createService(std::string className, bool isDataModel){
		if(lokupTable[className]){
			ClassMaker* maker = lokupTable[className];
			std::cout << maker << std::endl;
			if(maker->isService(isDataModel)){
				return maker->getInstance();
			}
		}

		return NULL;
	}

	std::vector<std::string> Factory::getRegisteredMetatables(){
		return registered;
	}

	bool Factory::isA(const ob_instance::Instance* obj, std::string className){
		if(lokupTable[className]){
			ClassMaker* maker = lokupTable[className];
			return maker->isA(obj);
		}
		return false;
	}

	void Factory::releaseTable(){}
}
