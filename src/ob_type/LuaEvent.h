#ifndef OB_TYPE_LuaEvent_H_
#define OB_TYPE_LuaEvent_H_

#include "../openblox/OpenBlox.h"
#define lua_evt_name "luaL_LuaEvent"

namespace ob_type {

class LuaEvent {
	public:
		LuaEvent(char* EventName);
		virtual ~LuaEvent();
		DECLARE_STATIC_INIT(LuaEvent);
		int wrap_lua(lua_State* L);
		void Fire();
		char* LuaEventName;
	private:
		static int lua_toString(lua_State* L);
		static int lua_connect(lua_State* L);
		std::vector<lua_State*> Connections;
};

} /* namespace ob_type */

#endif /* OB_TYPE_LuaEvent_H_ */
