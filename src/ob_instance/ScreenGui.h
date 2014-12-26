#ifndef OB_INSTANCE_SCREENGUI_H_
#define OB_INSTANCE_SCREENGUI_H_

#include "LayerCollector.h"

namespace ob_instance{
	class ScreenGui: public LayerCollector{
		public:
			ScreenGui();
			virtual ~ScreenGui();

			virtual void render();

			bool isMouseCaptured(int x, int y);

			virtual Instance* cloneImpl();

			virtual char* getClassName();

			virtual int wrap_lua(lua_State* L);

			DECLARE_STATIC_INIT(ScreenGui);
		protected:
			static char* ClassName;
			static char* LuaClassName;

			virtual void removeChild(Instance* kid);
			virtual void addChild(Instance* kid);
	};
};
#endif
