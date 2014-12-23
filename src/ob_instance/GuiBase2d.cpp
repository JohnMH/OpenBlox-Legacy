#include "GuiBase2d.h"

namespace ob_instance{
	struct GuiBase2dClassMaker: public OpenBlox::ClassMaker{
		ob_instance::Instance* getInstance() const{
			return NULL;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const GuiBase2d*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return false;
		}

		bool isService(bool isDataModel){
			return false;
		}
	};

	STATIC_INIT(GuiBase2d){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new GuiBase2dClassMaker());

		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, Instance::register_lua_events);
	}

	char* GuiBase2d::ClassName = "GuiBase2d";
	char* GuiBase2d::LuaClassName = "luaL_Instance_GuiBase2d";

	GuiBase2d::GuiBase2d() : GuiBase(){
		Name = ClassName;
	}

	GuiBase2d::~GuiBase2d(){}

	char* GuiBase2d::getClassName(){
		return ClassName;
	}
}
