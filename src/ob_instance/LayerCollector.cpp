#include "LayerCollector.h"

namespace ob_instance{
	struct LayerCollectorClassMaker: public OpenBlox::ClassMaker{
		ob_instance::Instance* getInstance() const{
			return NULL;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const LayerCollector*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return false;
		}

		bool isService(bool isDataModel){
			return false;
		}
	};

	STATIC_INIT(LayerCollector){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new LayerCollectorClassMaker());

		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
	}
	char* LayerCollector::ClassName = "LayerCollector";
	char* LayerCollector::LuaClassName = "luaL_Instance_LayerCollector";

	LayerCollector::LayerCollector() : GuiBase2d(){
		Name = ClassName;
	}

	LayerCollector::~LayerCollector(){}

	char* LayerCollector::getClassName(){
		return ClassName;
	}
}
