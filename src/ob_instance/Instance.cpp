#include "Instance.h"

namespace ob_instance{
	STATIC_INIT(Instance){

	}

	Instance::Instance(){

	}

	Instance::~Instance(){

	}

	Instance* Instance::newInstance(){
		return new Instance();
	}
}
