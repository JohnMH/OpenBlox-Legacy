#ifndef OB_INSTANCE_INSTANCE_H_
#define OB_INSTANCE_INSTANCE_H_

#include "Instances.h"

namespace ob_instance{
	class Instance{
		public:
			virtual void wrap_lua_impl(lua_State *L) = 0;
			virtual void wrap_lua(lua_State *L);
	};
}


#endif
