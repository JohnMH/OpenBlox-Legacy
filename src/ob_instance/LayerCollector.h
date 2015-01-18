#ifndef OB_INSTANCE_LAYERCOLLECTOR_H_
#define OB_INSTANCE_LAYERCOLLECTOR_H_

#include "GuiBase2d.h"

namespace ob_instance{
	class LayerCollector: public GuiBase2d{
		public:
			LayerCollector();
			virtual ~LayerCollector();

			DECLARE_CLASS(LayerCollector);
	};
};
#endif
