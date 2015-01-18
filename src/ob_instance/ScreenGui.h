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

			DECLARE_CLASS(ScreenGui);

			virtual void removeChild(Instance* kid);
			virtual void addChild(Instance* kid);
	};
};
#endif
