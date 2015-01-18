#include "LayerCollector.h"

namespace ob_instance{
	DEFINE_ABS_CLASS(LayerCollector);

	LayerCollector::LayerCollector() : GuiBase2d(){
		Name = ClassName;
	}

	LayerCollector::~LayerCollector(){}

	Instance* LayerCollector::cloneImpl(){
		return NULL;
	}
}
