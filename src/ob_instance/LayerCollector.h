#ifndef OB_INSTANCE_LAYERCOLLECTOR_H_
#define OB_INSTANCE_LAYERCOLLECTOR_H_

#include "GuiBase2d.h"

namespace ob_instance{
	class LayerCollector: public GuiBase2d{
		public:
			LayerCollector();
			virtual ~LayerCollector();

			virtual char* getClassName();

			DECLARE_STATIC_INIT(LayerCollector);
		protected:
			static char* ClassName;
			static char* LuaClassName;
	};
};
#endif
