#ifndef OB_INSTANCE_STARTERGUI_H_
#define OB_INSTANCE_STARTERGUI_H_

#include "BasePlayerGui.h"

namespace ob_instance{
	class StarterGui: public BasePlayerGui{
		public:
			StarterGui();
			virtual ~StarterGui();

			void sizeChanged(int width, int height);

			bool isMouseCaptured(int x, int y);

			virtual void Destroy();

			virtual void render();

			virtual Instance* cloneImpl();

			virtual std::string getClassName();

			virtual int wrap_lua(lua_State* L);

			DECLARE_STATIC_INIT(StarterGui);
		protected:
			bool ShowDevelopmentGui;

			static std::string ClassName;
			static std::string LuaClassName;

			virtual void removeChild(Instance* kid);
			virtual void addChild(Instance* kid);
	};
};
#endif
