#include "NetworkServer.h"

#include "../enet/enet.h"

#ifdef OPENBLOX_SERVER
namespace ob_instance{
	struct NetworkServerClassMaker: public OpenBlox::ClassMaker{
		ob_instance::Instance* getInstance() const{
			return new NetworkServer;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const NetworkServer*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return false;
		}

		bool isService(bool isDataModel){
			return isDataModel;
		}
	};

	STATIC_INIT(NetworkServer){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new NetworkServerClassMaker());

		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
	}

	std::string NetworkServer::ClassName = "NetworkServer";
	std::string NetworkServer::LuaClassName = "luaL_Instance_NetworkServer";

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

	int NetworkServer::wrap_lua(lua_State* L){
		NetworkServer** udata = (NetworkServer**)lua_newuserdata(L, sizeof(*this));
		*udata = this;

		luaL_getmetatable(L, LuaClassName.c_str());
		lua_setmetatable(L, -2);

		return 1;
	}

	Instance* NetworkServer::cloneImpl(){
		return NULL;
	}

	std::string NetworkServer::getClassName(){
		return ClassName;
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
