#include "GuiBase.h"

namespace ob_instance{
	DEFINE_ABS_CLASS(GuiBase);

	GuiBase::GuiBase() : Instance(){
		Name = ClassName;
	}

	GuiBase::~GuiBase(){}

	Instance* GuiBase::cloneImpl(){
		return NULL;
	}
}
