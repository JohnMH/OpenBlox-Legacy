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

		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
	}

	std::string Camera::ClassName = "Camera";
	std::string Camera::LuaClassName = "luaL_Instance_Camera";

	Camera::Camera() : Instance(){
		Name = ClassName;

		CameraType = ob_enum::CameraType::FIXED;
	}

	Camera::~Camera(){}

	int Camera::wrap_lua(lua_State* L){
		Camera** udata = (Camera**)lua_newuserdata(L, sizeof(*this));
		*udata = this;

		luaL_getmetatable(L, LuaClassName.c_str());
		lua_setmetatable(L, -2);

		return 1;
	}

	Instance* Camera::cloneImpl(){
		Camera* newGuy = new Camera;
		newGuy->Name = Name;
		return newGuy;
	}

	std::string Camera::getClassName(){
		return ClassName;
	}

	int Camera::lua_getCameraType(lua_State* L){
		Camera* cam = (Camera*)Instance::checkInstance(L, 1);
		ob_enum::LuaEnumItem* val = ob_enum::LuaCameraType->GetEnumItem((int)cam->CameraType);
		return val->wrap_lua(L);
	}

	int Camera::lua_setCameraType(lua_State* L){
		Camera* cam = (Camera*)Instance::checkInstance(L, 1);
		ob_enum::LuaEnumItem* val = ob_enum::checkEnumItem(L, 2, ob_enum::LuaCameraType);
		cam->CameraType = (ob_enum::CameraType)val->Value;
		return 0;
	}

	void Camera::register_lua_property_getters(lua_State* L){
		luaL_Reg props[]{
			{"CameraType", lua_getCameraType},
			{NULL, NULL}
		};
		luaL_register(L, NULL, props);

		Instance::register_lua_property_getters(L);
	}

	void Camera::register_lua_property_setters(lua_State* L){
		luaL_Reg props[]{
			{"CameraType", lua_setCameraType},
			{NULL, NULL}
		};
		luaL_register(L, NULL, props);

		Instance::register_lua_property_setters(L);
	}
}
