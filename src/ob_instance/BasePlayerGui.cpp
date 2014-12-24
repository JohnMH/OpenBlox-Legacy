#include "BasePlayerGui.h"

namespace ob_instance{
	struct BasePlayerGuiClassMaker: public OpenBlox::ClassMaker{
		ob_instance::Instance* getInstance() const{
			return NULL;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const BasePlayerGui*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return false;
		}

		bool isService(bool isDataModel){
			return false;
		}
	};

	STATIC_INIT(BasePlayerGui){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new BasePlayerGuiClassMaker());

		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
	}

	char* BasePlayerGui::ClassName = "BasePlayerGui";
	char* BasePlayerGui::LuaClassName = "luaL_Instance_BasePlayerGui";

	BasePlayerGui::BasePlayerGui() : Instance(){
		Name = ClassName;
	}

	BasePlayerGui::~BasePlayerGui(){}

	char* BasePlayerGui::getClassName(){
		return ClassName;
	}
}
