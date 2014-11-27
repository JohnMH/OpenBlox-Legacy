#include "Instance.h"

namespace ob_instance{
	struct InstanceClassMaker: public OpenBlox::ClassMaker{
		void* getInstance() const{
			return NULL;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const Instance*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return false;
		}
	};

	STATIC_INIT(Instance){
		OpenBlox::BaseGame::getInstanceFactory()->addClass("Instance", new InstanceClassMaker());
	}

	bool Instance::IsA(const char* name){
		return OpenBlox::BaseGame::getInstanceFactory()->isA(this, name);
	}
}
