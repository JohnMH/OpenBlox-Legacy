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

		bool isService(){
			return false;
		}
	};

	STATIC_INIT(Camera){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new CameraClassMaker());

		lua_State* L = OpenBlox::BaseGame::getGlobalState();

		luaL_newmetatable(L, LuaClassName);

		register_lua_metamethods(L);

		lua_pushstring(L, "__index");
		lua_newtable(L);

		lua_pushstring(L, "ClassName");
		lua_pushstring(L, ClassName);
		lua_rawset(L, -3);

		register_lua_methods(L);

		lua_rawset(L, -3);
		lua_pop(L, 1);
	}

	char* Camera::ClassName = "Camera";
	char* Camera::LuaClassName = "luaL_Instance_Camera";

	Camera::Camera() : Instance(){
		Name = ClassName;
	}

	Camera::~Camera(){

	}

	int Camera::wrap_lua(lua_State* L){
		Camera** udata = (Camera**)lua_newuserdata(L, sizeof(Camera*));
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
}
