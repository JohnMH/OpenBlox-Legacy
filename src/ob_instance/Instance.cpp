#include "Instance.h"

namespace ob_instance{
	Instance::Instance(){

	}

	Instance::~Instance(){

	}

	Instance* Instance::newInstance(){
		return new Instance();
	}
}
