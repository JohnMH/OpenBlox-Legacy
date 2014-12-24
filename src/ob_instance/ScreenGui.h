#ifndef OB_INSTANCE_SCREENGUI_H_
#define OB_INSTANCE_SCREENGUI_H_

#include "LayerCollector.h"

namespace ob_instance{
	class ScreenGui: public LayerCollector{
		public:
			ScreenGui();
			virtual ~ScreenGui();

			virtual void render();

			virtual Instance* cloneImpl();

			virtual char* getClassName();

			virtual int wrap_lua(lua_State* L);

			DECLARE_STATIC_INIT(ScreenGui);
		protected:
			static char* ClassName;
			static char* LuaClassName;
	};
};
#endif
