#ifndef OB_INSTANCE_NETWORKPEER_H_
#define OB_INSTANCE_NETWORKPEER_H_

#include "Instance.h"

namespace ob_instance{
	class NetworkPeer: public Instance{
		public:
		NetworkPeer();
			virtual ~NetworkPeer();

			virtual void SetOutgoingKBPSLimit(int limit) = 0;

			virtual Instance* cloneImpl();

			virtual std::string getClassName();

			virtual int wrap_lua(lua_State* L);

			DECLARE_STATIC_INIT(NetworkPeer);
		protected:
			static std::string ClassName;
			static std::string LuaClassName;

			static int lua_setOutgoingKBPSLimit(lua_State* L);

			static void register_lua_methods(lua_State* L);
	};
}
#endif
