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

			virtual std::string getClassName();

			virtual int wrap_lua(lua_State* L);

			DECLARE_STATIC_INIT(Frame);
		protected:
			static std::string ClassName;
			static std::string LuaClassName;

			//Enum FrameStyle Style
	};
}
#endif
