#ifndef OB_INSTANCE_LAYERCOLLECTOR_H_
#define OB_INSTANCE_LAYERCOLLECTOR_H_

#include "GuiBase2d.h"

namespace ob_instance{
	class LayerCollector: public GuiBase2d{
		public:
			LayerCollector();
			virtual ~LayerCollector();

			virtual std::string getClassName();

			DECLARE_STATIC_INIT(LayerCollector);
		protected:
			static std::string ClassName;
			static std::string LuaClassName;
	};
};
#endif
