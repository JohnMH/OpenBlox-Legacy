#include "ServiceProvider.h"

namespace ob_instance{
	DEFINE_ABS_CLASS(ServiceProvider);

	ServiceProvider::ServiceProvider() : Instance(){
		Name = ClassName;
	}

	ServiceProvider::~ServiceProvider(){}

	Instance* ServiceProvider::cloneImpl(){
		return NULL;
	}

	Instance* ServiceProvider::FindService(std::string className){
		for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
			Instance* kid = children[i];
			if(kid != NULL){
				if(kid->getClassName() == className){
					return kid;
				}
			}
		}
		return NULL;
	}

	Instance* ServiceProvider::GetService(std::string className){
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
			std::string serviceName = std::string(luaL_checkstring(L, 2));
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
			std::string serviceName = std::string(luaL_checkstring(L, 2));
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
