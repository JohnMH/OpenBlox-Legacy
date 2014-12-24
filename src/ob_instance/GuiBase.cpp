#include "GuiBase.h"

namespace ob_instance{
	struct GuiBaseClassMaker: public OpenBlox::ClassMaker{
		ob_instance::Instance* getInstance() const{
			return NULL;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const GuiBase*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return false;
		}

		bool isService(bool isDataModel){
			return false;
		}
	};

	STATIC_INIT(GuiBase){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new GuiBaseClassMaker());

		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
	}

	char* GuiBase::ClassName = "GuiBase";
	char* GuiBase::LuaClassName = "luaL_Instance_GuiBase";

	GuiBase::GuiBase() : Instance(){
		Name = ClassName;
	}

	GuiBase::~GuiBase(){}

	char* GuiBase::getClassName(){
		return ClassName;
	}
}
