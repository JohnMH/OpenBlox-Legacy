#ifndef OB_TYPE_LuaEvent_H_
#define OB_TYPE_LuaEvent_H_

#include "../openblox/OpenBlox.h"
#define lua_evt_name "luaL_LuaEvent"
#define lua_evt_con_name "luaL_LuaEventConnection"

namespace ob_type{
	class LuaEvent;
	class LuaEventConnection{
		public:
			LuaEventConnection(LuaEvent* evt, int ref);
			virtual ~LuaEventConnection();

			void disconnect();

			DECLARE_STATIC_INIT(LuaEventConnection);

			int wrap_lua(lua_State* L);

			static int lua_toString(lua_State* L);
			static int lua_disconnect(lua_State* L);
			static int lua_index(lua_State* L);
			static int lua_newindex(lua_State* L);
			static int lua_getConnected(lua_State* L);
		protected:
			LuaEvent* evt;
			int ref;
			bool connected;

	};

	class LuaEvent{
		public:
			LuaEvent(const char* EventName);
			virtual ~LuaEvent();

			void disconnectAll();
			void disconnect(int ref);
			bool isConnected(int ref);

			DECLARE_STATIC_INIT(LuaEvent);

			int wrap_lua(lua_State* L);

			typedef void (*luaFireFunc)(lua_State*, va_list);
			void Fire(luaFireFunc fireFunc, int args, ...);
		private:
			std::vector<int> connections;
			const char* LuaEventName;

			static int lua_toString(lua_State* L);
			static int lua_connect(lua_State* L);
	};
}
#endif