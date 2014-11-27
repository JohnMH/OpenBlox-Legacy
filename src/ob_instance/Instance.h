#ifndef OB_INSTANCE_INSTANCE_H_
#define OB_INSTANCE_INSTANCE_H_

#include "../openblox/OpenBlox.h"

namespace ob_instance{
	class Instance{
		public:
			virtual bool IsA(const char* name);
			virtual void wrap_lua(lua_State *L) = 0;

			DECLARE_STATIC_INIT(Instance);
	};
}

#endif
