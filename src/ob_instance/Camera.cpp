#include "Camera.h"

namespace ob_instance{
	DEFINE_CLASS(Camera, true, false);

	Camera::Camera() : Instance(){
		Name = ClassName;

		CameraType = ob_enum::CameraType::FIXED;
	}

	Camera::~Camera(){}

	Instance* Camera::cloneImpl(){
		Camera* newGuy = new Camera;
		newGuy->Name = Name;
		return newGuy;
	}

	void Camera::serialize_impl(rapidjson::Writer<rapidjson::StringBuffer>* writer){
		Instance::serialize_impl(writer);

		writer->String("CameraType");
		writer->Int((int)CameraType);
	}

	int Camera::lua_getCameraType(lua_State* L){
		Instance* inst = Instance::checkInstance(L, 1);
		if (Camera* cam = dynamic_cast<Camera*>(inst)){
			ob_enum::LuaEnumItem* val = ob_enum::LuaCameraType->GetEnumItem((int)cam->CameraType);
			return val->wrap_lua(L);
		}
		return 0;
	}

	int Camera::lua_setCameraType(lua_State* L){
		Instance* inst = Instance::checkInstance(L, 1);
		if (Camera* cam = dynamic_cast<Camera*>(inst)){
			ob_enum::LuaEnumItem* val = ob_enum::checkEnumItem(L, 2, ob_enum::LuaCameraType);
			cam->CameraType = (ob_enum::CameraType)val->Value;
			propertyChanged("CameraType", cam);
		}
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
