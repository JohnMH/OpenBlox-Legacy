#ifndef OB_INSTANCE_GUIOBJECT_H_
#define OB_INSTANCE_GUIOBJECT_H_

#include "GuiBase2d.h"

namespace ob_instance{
	class GuiObject: public GuiBase2d{
		public:
			GuiObject();
			virtual ~GuiObject();

			DECLARE_STATIC_INIT(GuiObject);
		protected:
			static char* ClassName;
			static char* LuaClassName;
	};
};
#endif
