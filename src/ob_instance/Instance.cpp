#include "Instance.h"

namespace ob_instance{
	STATIC_INIT(Instance){
	    std::cout << "Static Initializer" << std::endl;
	}

	Instance::Instance(){

	}

	Instance::~Instance(){

	}
}
