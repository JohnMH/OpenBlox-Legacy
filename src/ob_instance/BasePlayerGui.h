#ifndef OB_INSTANCE_BASEPLAYERGUI_H_
#define OB_INSTANCE_BASEPLAYERGUI_H_

#include "Instance.h"

namespace ob_instance{
	class BasePlayerGui: public Instance{
		public:
			BasePlayerGui();
			virtual ~BasePlayerGui();

			DECLARE_CLASS(BasePlayerGui);
	};
}
#endif
