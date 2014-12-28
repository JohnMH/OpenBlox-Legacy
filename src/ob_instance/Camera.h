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

			DECLARE_STATIC_INIT(Camera);
		protected:
			static std::string ClassName;
			static std::string LuaClassName;
	};
}
#endif
