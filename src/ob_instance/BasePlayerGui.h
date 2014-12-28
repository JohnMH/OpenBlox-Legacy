#ifndef OB_INSTANCE_BASEPLAYERGUI_H_
#define OB_INSTANCE_BASEPLAYERGUI_H_

#include "Instance.h"

namespace ob_instance{
	class BasePlayerGui: public Instance{
		public:
			BasePlayerGui();
			virtual ~BasePlayerGui();

			virtual std::string getClassName();

			DECLARE_STATIC_INIT(BasePlayerGui);
		protected:
			static std::string ClassName;
			static std::string LuaClassName;
	};
}
#endif
