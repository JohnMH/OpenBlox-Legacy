#ifndef OB_INSTANCE_SCREENGUI_H_
#define OB_INSTANCE_SCREENGUI_H_

#include "LayerCollector.h"

namespace ob_instance{
	class GuiObject;

	class ScreenGui: public LayerCollector{
		public:
			ScreenGui();
			virtual ~ScreenGui();

			virtual void render();

			void giveUpKids(std::vector<GuiObject*>* kids);

			DECLARE_CLASS(ScreenGui);

			virtual void removeChild(Instance* kid);
			virtual void addChild(Instance* kid);
	};
};
#endif
