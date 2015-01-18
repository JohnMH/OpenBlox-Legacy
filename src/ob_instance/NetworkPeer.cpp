#include "NetworkPeer.h"

namespace ob_instance{
	DEFINE_ABS_CLASS(NetworkPeer);

	NetworkPeer::NetworkPeer() : Instance(){
		Name = ClassName;
	}

	NetworkPeer::~NetworkPeer(){}

	Instance* NetworkPeer::cloneImpl(){
		return NULL;
	}

	int NetworkPeer::lua_setOutgoingKBPSLimit(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(NetworkPeer* np = dynamic_cast<NetworkPeer*>(inst)){
			int limit = luaL_checkint(L, 2);

			np->SetOutgoingKBPSLimit(limit);

			return 0;
		}
		return luaL_error(L, COLONERR, "SetOutgoingKBPSLimit");
	}

	void NetworkPeer::register_lua_methods(lua_State* L){
		luaL_Reg props[]{
			{"SetOutgoingKBPSLimit", lua_setOutgoingKBPSLimit},
			{NULL, NULL}
		};
		luaL_register(L, NULL, props);

		Instance::register_lua_methods(L);
	}
}
