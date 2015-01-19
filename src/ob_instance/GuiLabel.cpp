#include "GuiLabel.h"

namespace ob_instance{
	DEFINE_ABS_CLASS(GuiLabel);

	GuiLabel::GuiLabel() : GuiObject(){
		Name = ClassName;
	}

	GuiLabel::~GuiLabel(){}

	Instance* GuiLabel::cloneImpl(){
		return NULL;
	}
}
