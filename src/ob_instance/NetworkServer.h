#ifndef OB_INSTANCE_NETWORKSERVER_H_
#define OB_INSTANCE_NETWORKSERVER_H_

#include "NetworkPeer.h"

#ifdef OPENBLOX_SERVER
namespace ob_instance{
	class NetworkServer: public NetworkPeer{
		public:
			NetworkServer();
			virtual ~NetworkServer();

			virtual void SetOutgoingKBPSLimit(int limit);

			int GetClientCount();
			void Start(int port = 53640, int threadSleepTime = 20);
			void Stop(int blockDuration = 1000);
			int GetPort();

			virtual Instance* cloneImpl();

			virtual std::string getClassName();

			virtual int wrap_lua(lua_State* L);

			DECLARE_STATIC_INIT(NetworkServer);
		protected:
			int Port;

			static std::string ClassName;
			static std::string LuaClassName;

			static void register_lua_property_getters(lua_State* L);
			static void register_lua_property_setters(lua_State* L);
	};
}
#endif
#endif
