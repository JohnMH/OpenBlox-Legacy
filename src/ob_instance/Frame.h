#ifndef OB_INSTANCE_FRAME_H_
#define OB_INSTANCE_FRAME_H_

#include "GuiObject.h"

namespace ob_instance{
	class Frame: public GuiObject{
		public:
			Frame();
			virtual ~Frame();

			virtual void render();

			virtual Instance* cloneImpl();

			virtual char* getClassName();

			virtual int wrap_lua(lua_State* L);

			DECLARE_STATIC_INIT(Frame);
		protected:
			static char* ClassName;
			static char* LuaClassName;

			//Enum FrameStyle Style
	};
}
#endif
