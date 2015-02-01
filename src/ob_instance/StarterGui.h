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

			void onClick(int x, int y, ob_enum::MouseButton button, bool down);

			DECLARE_CLASS(StarterGui);

			bool ShowDevelopmentGui;

			virtual void removeChild(Instance* kid);
			virtual void addChild(Instance* kid);
	};
};
#endif
