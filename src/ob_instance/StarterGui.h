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

			DECLARE_CLASS(StarterGui);

			bool ShowDevelopmentGui;

			virtual void removeChild(Instance* kid);
			virtual void addChild(Instance* kid);
	};
};
#endif
