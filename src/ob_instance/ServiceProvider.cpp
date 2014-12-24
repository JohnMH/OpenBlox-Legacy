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

		bool isService(bool isDataModel){
			return false;
		}
	};

	STATIC_INIT(ServiceProvider){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new ServiceProviderClassMaker());

		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
	}

	char* ServiceProvider::ClassName = "ServiceProvider";
	char* ServiceProvider::LuaClassName = "luaL_Instance_ServiceProvider";

	ServiceProvider::ServiceProvider() : Instance(){
		Name = ClassName;
	}

	ServiceProvider::~ServiceProvider(){}

	Instance* ServiceProvider::FindService(const char* className){
		for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
			Instance* kid = children[i];
			if(kid != NULL){
				if(strcmp(kid->getClassName(), className) == 0){
					return kid;
				}
			}
		}
		return NULL;
	}

	Instance* ServiceProvider::GetService(const char* className){
		Instance* foundService = FindService(className);
		if(foundService != NULL){
			return foundService;
		}
		ob_instance::Instance* newGuy = OpenBlox::BaseGame::getInstanceFactory()->createService(className, false);
		if(newGuy){
			newGuy->setParent(this);
			newGuy->parentLock();
		}
		return newGuy;
	}

	char* ServiceProvider::getClassName(){
		return ClassName;
	}

	void ServiceProvider::register_lua_methods(lua_State* L){
		Instance::register_lua_methods(L);
		luaL_Reg methods[]{
			{"FindService", lua_FindService},
			{"GetService", lua_GetService},
			{NULL, NULL}
		};
		luaL_register(L, NULL, methods);
	}

	int ServiceProvider::lua_FindService(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(ServiceProvider* sp = dynamic_cast<ServiceProvider*>(inst)){
			const char* serviceName = luaL_checkstring(L, 2);
			Instance* foundGuy = sp->FindService(serviceName);
			if(foundGuy != NULL){
				return foundGuy->wrap_lua(L);
			}
			lua_pushnil(L);
			return 0;
		}
		return luaL_error(L, COLONERR, "FindService");
	}

	int ServiceProvider::lua_GetService(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(ServiceProvider* sp = dynamic_cast<ServiceProvider*>(inst)){
			const char* serviceName = luaL_checkstring(L, 2);
			Instance* foundGuy = sp->GetService(serviceName);
			if(foundGuy != NULL){
				return foundGuy->wrap_lua(L);
			}
			lua_pushnil(L);
			return 0;
		}
		return luaL_error(L, COLONERR, "GetService");
	}
}
