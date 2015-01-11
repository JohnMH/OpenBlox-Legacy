#include "NetworkPeer.h"

namespace ob_instance{
	struct NetworkPeerClassMaker: public OpenBlox::ClassMaker{
		ob_instance::Instance* getInstance() const{
			return NULL;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const NetworkPeer*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return false;
		}

		bool isService(bool isDataModel){
			return false;
		}
	};

	STATIC_INIT(NetworkPeer){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new NetworkPeerClassMaker());

		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
	}

	std::string NetworkPeer::ClassName = "NetworkPeer";
	std::string NetworkPeer::LuaClassName = "luaL_Instance_NetworkPeer";

	NetworkPeer::NetworkPeer() : Instance(){
		Name = ClassName;
	}

	NetworkPeer::~NetworkPeer(){}

	int NetworkPeer::wrap_lua(lua_State* L){
		NetworkPeer** udata = (NetworkPeer**)lua_newuserdata(L, sizeof(*this));
		*udata = this;

		luaL_getmetatable(L, LuaClassName.c_str());
		lua_setmetatable(L, -2);

		return 1;
	}

	Instance* NetworkPeer::cloneImpl(){
		return NULL;
	}

	std::string NetworkPeer::getClassName(){
		return ClassName;
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
