#include "Instance.h"

namespace ob_instance{
	void Instance::wrap_lua(lua_State *L){
		Instance ** udata = lua_newuserdata(L, sizeof(this));
		*udata = this;

		wrap_lua_impl(L);
	}
}
