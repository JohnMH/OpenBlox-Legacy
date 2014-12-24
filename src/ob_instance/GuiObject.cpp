#include "GuiObject.h"

namespace ob_instance{
	struct GuiObjectClassMaker: public OpenBlox::ClassMaker{
		ob_instance::Instance* getInstance() const{
			return NULL;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const GuiObject*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return false;
		}

		bool isService(bool isDataModel){
			return false;
		}
	};

	STATIC_INIT(GuiObject){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new GuiObjectClassMaker());

		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
	}

	char* GuiObject::ClassName = "GuiObject";
	char* GuiObject::LuaClassName = "luaL_Instance_GuiObject";

	GuiObject::GuiObject() : GuiBase2d(){}
	GuiObject::~GuiObject(){}
}
