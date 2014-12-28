#ifndef OB_INSTANCE_CAMERA_H_
#define OB_INSTANCE_CAMERA_H_

#include "Instance.h"

namespace ob_instance{
	class Camera: public Instance{
		public:
			Camera();
			virtual ~Camera();

			virtual Instance* cloneImpl();

			virtual std::string getClassName();

			virtual int wrap_lua(lua_State* L);

			static int lua_getCameraType(lua_State* L);
			static int lua_setCameraType(lua_State* L);

			DECLARE_STATIC_INIT(Camera);
		protected:
			static std::string ClassName;
			static std::string LuaClassName;

			ob_enum::CameraType CameraType;

			static void register_lua_property_getters(lua_State* L);
			static void register_lua_property_setters(lua_State* L);
	};
}
#endif
