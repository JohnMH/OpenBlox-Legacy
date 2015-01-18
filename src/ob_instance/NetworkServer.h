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

			DECLARE_CLASS(NetworkServer);

			int Port;

			static void register_lua_property_getters(lua_State* L);
			static void register_lua_property_setters(lua_State* L);
	};
}
#endif
#endif
