#ifndef OB_INSTANCE_GUIBASE2D_H_
#define OB_INSTANCE_GUIBASE2D_H_

#include "GuiBase.h"

namespace ob_instance{
	class GuiBase2d: public GuiBase{
		public:
			GuiBase2d();
			virtual ~GuiBase2d();

			virtual char* getClassName();

			DECLARE_STATIC_INIT(GuiBase2d);
		protected:
			static char* ClassName;
			static char* LuaClassName;
	};
};
#endif
