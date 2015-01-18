#ifndef OB_INSTANCE_CAMERA_H_
#define OB_INSTANCE_CAMERA_H_

#include "Instance.h"

namespace ob_instance{
	class Camera: public Instance{
		public:
			Camera();
			virtual ~Camera();

			virtual void serialize_impl(rapidjson::Writer<rapidjson::StringBuffer>* writer);

			static int lua_getCameraType(lua_State* L);
			static int lua_setCameraType(lua_State* L);

			DECLARE_CLASS(Camera);

			ob_enum::CameraType CameraType;

			static void register_lua_property_getters(lua_State* L);
			static void register_lua_property_setters(lua_State* L);
	};
}
#endif
