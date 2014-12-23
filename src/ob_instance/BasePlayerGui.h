#ifndef OB_INSTANCE_BASEPLAYERGUI_H_
#define OB_INSTANCE_BASEPLAYERGUI_H_

#include "Instance.h"

namespace ob_instance{
	class BasePlayerGui: public Instance{
		public:
			BasePlayerGui();
			virtual ~BasePlayerGui();

			virtual char* getClassName();

			DECLARE_STATIC_INIT(BasePlayerGui);
		protected:
			static char* ClassName;
			static char* LuaClassName;
	};
}
#endif
