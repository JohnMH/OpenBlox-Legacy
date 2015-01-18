#include "BasePlayerGui.h"

namespace ob_instance{
	DEFINE_ABS_CLASS(BasePlayerGui);

	BasePlayerGui::BasePlayerGui() : Instance(){
		Name = ClassName;
	}

	BasePlayerGui::~BasePlayerGui(){}

	Instance* BasePlayerGui::cloneImpl(){
		return NULL;
	}
}
