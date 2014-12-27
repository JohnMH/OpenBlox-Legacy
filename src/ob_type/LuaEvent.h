#ifndef OB_TYPE_LuaEvent_H_
#define OB_TYPE_LuaEvent_H_

#include "../openblox/OpenBlox.h"

#define lua_evt_name "luaL_LuaEvent"
#define lua_evt_con_name "luaL_LuaEventConnection"

namespace ob_instance{
	class Instance;
}

namespace ob_type{
	enum VarType{
		TYPE_INT,
		TYPE_DOUBLE,
		TYPE_BOOL,
		TYPE_CONST_CHAR,
		TYPE_CHAR,
		TYPE_INSTANCE,
		TYPE_UNKNOWN
	};

	class VarWrapper{
		public:
			VarWrapper();
			VarWrapper(int var);
			VarWrapper(double var);
			VarWrapper(bool var);
			VarWrapper(char* var);
			VarWrapper(const char* var);
			VarWrapper(ob_instance::Instance* var);
			~VarWrapper();

			void* wrapped;
			VarType type;
	};

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

	LuaEventConnection* checkEventConnection(lua_State* L, int n);

	class LuaEvent{
		public:
			struct EvtCon{
				lua_State* env;
				int ref;
			};
			LuaEvent(const char* EventName, int nargs);
			virtual ~LuaEvent();

			void disconnectAll();
			void disconnect(int ref);
			bool isConnected(int ref);

			DECLARE_STATIC_INIT(LuaEvent);

			int wrap_lua(lua_State* L);

			static void pushWrappersToLua(lua_State* L, std::vector<VarWrapper> argList);

			void Fire(std::vector<VarWrapper> argList);
		private:
			std::vector<EvtCon> connections;
			std::vector<lua_State*> waiting;
			const char* LuaEventName;
			int nargs;

			static int lua_toString(lua_State* L);
			static int lua_connect(lua_State* L);
			static int lua_wait(lua_State* L);
	};

	LuaEvent* checkEvent(lua_State* L, int n);
}
#endif
