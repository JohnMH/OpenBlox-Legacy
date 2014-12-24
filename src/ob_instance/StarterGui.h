#ifndef OB_INSTANCE_STARTERGUI_H_
#define OB_INSTANCE_STARTERGUI_H_

#include "BasePlayerGui.h"

namespace ob_instance{
	class StarterGui: public BasePlayerGui{
		public:
			StarterGui();
			virtual ~StarterGui();

			void sizeChanged(int width, int height);

			virtual void Destroy();

			virtual void render();

			virtual Instance* cloneImpl();

			virtual char* getClassName();

			virtual int wrap_lua(lua_State* L);

			DECLARE_STATIC_INIT(StarterGui);
		protected:
			bool ShowDevelopmentGui;

			static char* ClassName;
			static char* LuaClassName;

			virtual void removeChild(Instance* kid);
			virtual void addChild(Instance* kid);
	};
};
#endif
