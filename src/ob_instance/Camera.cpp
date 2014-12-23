#include "Camera.h"

namespace ob_instance{
	struct CameraClassMaker: public OpenBlox::ClassMaker{
		ob_instance::Instance* getInstance() const{
			return new Camera;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const Camera*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return true;
		}

		bool isService(bool isDataModel){
			return false;
		}
	};

	STATIC_INIT(Camera){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new CameraClassMaker());

		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, Instance::register_lua_events);
	}

	char* Camera::ClassName = "Camera";
	char* Camera::LuaClassName = "luaL_Instance_Camera";

	Camera::Camera() : Instance(){
		Name = ClassName;
	}

	Camera::~Camera(){

	}

	int Camera::wrap_lua(lua_State* L){
		Camera** udata = (Camera**)lua_newuserdata(L, sizeof(*this));
		*udata = this;

		luaL_getmetatable(L, LuaClassName);
		lua_setmetatable(L, -2);

		return 1;
	}

	Instance* Camera::cloneImpl(){
		Camera* newGuy = new Camera;
		newGuy->Name = Name;
		return newGuy;
	}

	char* Camera::getClassName(){
		return ClassName;
	}
}
