#ifndef OB_INSTANCE_STARTERGUI_H_
#define OB_INSTANCE_STARTERGUI_H_

#include "BasePlayerGui.h"

namespace ob_instance{
	class StarterGui: public BasePlayerGui{
		public:
			StarterGui();
			virtual ~StarterGui();

			virtual void render();

			virtual Instance* cloneImpl();

			virtual char* getClassName();

			virtual int wrap_lua(lua_State* L);

			DECLARE_STATIC_INIT(StarterGui);
		protected:
			bool ShowDevelopmentGui;

			static char* ClassName;
			static char* LuaClassName;
	};
};
#endif
