/*
 * ServiceProvider.cpp
 *
 *  Created on: Nov 28, 2014
 *      Author: john
 */

#include "ServiceProvider.h"

namespace ob_instance{
	struct ServiceProviderClassMaker: public OpenBlox::ClassMaker{
		void* getInstance() const{
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

		Instance::register_lua_metamethods(L);

		lua_pushstring(L, "__index");
		lua_newtable(L);

		lua_pushstring(L, "ClassName");
		lua_pushstring(L, ClassName);
		lua_rawset(L, -3);

		register_lua_methods(L);

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
