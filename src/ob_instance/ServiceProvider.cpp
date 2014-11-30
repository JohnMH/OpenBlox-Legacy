/*
 * ServiceProvider.cpp
 *
 *  Created on: Nov 28, 2014
 *      Author: john
 */

#include "ServiceProvider.h"

namespace ob_instance{
	struct ServiceProviderClassMaker: public OpenBlox::ClassMaker{
		ob_instance::Instance* getInstance() const{
			return NULL;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const ServiceProvider*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return false;
		}

		bool isService(){
			return false;
		}
	};

	STATIC_INIT(ServiceProvider){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new ServiceProviderClassMaker());

		lua_State* L = OpenBlox::BaseGame::getGlobalState();

		luaL_newmetatable(L, LuaClassName);
		register_lua_metamethods(L);

		lua_pushstring(L, "__metatable");
		lua_pushstring(L, "This metatable is locked");
		lua_rawset(L, -3);

		// methods
		lua_pushstring(L, "__methods");
		lua_newtable(L);
		register_lua_methods(L);
		lua_rawset(L, -3);

		// property getters
		lua_pushstring(L, "__propertygetters");
		lua_newtable(L);
		register_lua_property_getters(L);
		lua_rawset(L, -3);

		// property setters
		lua_pushstring(L, "__propertysetters");
		lua_newtable(L);
		register_lua_property_setters(L);
		lua_rawset(L, -3);

		// item get
		lua_pushstring(L, "__index");
		lua_pushcfunction(L, lua_index);
		lua_rawset(L, -3);

		// item set
		lua_pushstring(L, "__newindex");
		lua_pushcfunction(L, lua_newindex);
		lua_rawset(L, -3);

		lua_pop(L, 1);
	}

	char* ServiceProvider::ClassName = "ServiceProvider";
	char* ServiceProvider::LuaClassName = "luaL_Instance_ServiceProvider";

	ServiceProvider::ServiceProvider() : Instance(){
		Name = ClassName;
	}

	ServiceProvider::~ServiceProvider(){

	}

	Instance* ServiceProvider::FindService(char* className){
		//Look through children for service
		return NULL;
	}

	Instance* ServiceProvider::GetService(char* className){
		Instance* foundService = FindService(className);
		if(foundService != NULL){
			return foundService;
		}
		//Create service using Factory::create and ClassMaker::isService.
		return NULL;
	}

	void ServiceProvider::register_lua_methods(lua_State* L){
		Instance::register_lua_methods(L);
	}
}
