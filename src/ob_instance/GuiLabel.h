#ifndef OB_INSTANCE_GUILABEL_H_
#define OB_INSTANCE_GUILABEL_H_

#include "GuiObject.h"

namespace ob_instance{
	class GuiLabel: public GuiObject{
		public:
			GuiLabel();
			virtual ~GuiLabel();

			DECLARE_CLASS(GuiLabel);
	};
}
#endif
