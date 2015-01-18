#ifndef OB_INSTANCE_FRAME_H_
#define OB_INSTANCE_FRAME_H_

#include "GuiObject.h"

namespace ob_instance{
	class Frame: public GuiObject{
		public:
			Frame();
			virtual ~Frame();

			virtual void render();

			DECLARE_CLASS(Frame);

			//Enum FrameStyle Style
	};
}
#endif
