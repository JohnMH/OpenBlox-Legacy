#ifndef OB_INSTANCE_GUIBASE_H_
#define OB_INSTANCE_GUIBASE_H_

#include "Instance.h"

namespace ob_instance{
	class GuiBase: public Instance{
		public:
			GuiBase();
			virtual ~GuiBase();

			virtual char* getClassName();

			DECLARE_STATIC_INIT(GuiBase);
		protected:
			static char* ClassName;
			static char* LuaClassName;
	};
};
#endif
