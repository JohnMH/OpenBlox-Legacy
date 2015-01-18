#include "NetworkServer.h"

#include "../enet/enet.h"

#ifdef OPENBLOX_SERVER
namespace ob_instance{
	DEFINE_CLASS(NetworkServer, false, true);

	static int outgoingKBPSLimit = 0;
	static int netThreadSleepTime = 20;
	static bool netWorking = false;
	static bool netWaiting = false;
	static ENetHost* server = NULL;
	static OpenBlox::Thread* netThread = NULL;

	NetworkServer::NetworkServer() : NetworkPeer(){
		Name = ClassName;

		Port = 0;
	}

	NetworkServer::~NetworkServer(){}

	void NetworkServer::SetOutgoingKBPSLimit(int limit){
		outgoingKBPSLimit = limit;
		enet_host_bandwidth_limit(server, 0, outgoingKBPSLimit * 1000);
	}

	int NetworkServer::GetClientCount(){

	}

	void NetworkServer::Start(int port, int threadSleepTime){
		if(server != NULL){
			LOGI("Attempt to Start server while running.");
			return;
		}

		Port = port;
		netThreadSleepTime = threadSleepTime;

		ENetAddress address;
		address.host = ENET_HOST_ANY;
		address.port = port;

		server = enet_host_create(&address,
			32,//32 max connections
			2, //Two channels
			0, //Infinite incoming bandwidth
			outgoingKBPSLimit * 1000 //bytes to KB
		);

		if(server == NULL){
			LOGI("[ENet] Failed to create server.");
			return;
		}

		netThread = new OpenBlox::Thread([](){
			ENetEvent event;

			while(enet_host_service(server, &event, 1000) > 0){
				netWorking = true;
				switch(event.type){
					case ENET_EVENT_TYPE_CONNECT:
						break;
				}
				netWorking = false;
				if(netWaiting){
					while(netWaiting){}
				}
				if(server == NULL){
					break;
				}
				usleep(10000);
			}
		});
		netThread->start();
	}

	void NetworkServer::Stop(int blockDuration){
		if(server == NULL){
			LOGI("Attempt to Stop server while not running.");
			return;
		}
		netWaiting = true;
		if(netWorking){
			while(netWorking){}
		}
		enet_host_destroy(server);
		server = NULL;

		netWorking = false;
		netWaiting = false;
	}

	int NetworkServer::GetPort(){
		return Port;
	}

	Instance* NetworkServer::cloneImpl(){
		return NULL;
	}

	void NetworkServer::register_lua_property_getters(lua_State* L){
		luaL_Reg props[]{
			{NULL, NULL}
		};
		luaL_register(L, NULL, props);

		Instance::register_lua_property_getters(L);
	}

	void NetworkServer::register_lua_property_setters(lua_State* L){
		luaL_Reg props[]{
			{NULL, NULL}
		};
		luaL_register(L, NULL, props);

		Instance::register_lua_property_setters(L);
	}
}
#endif
