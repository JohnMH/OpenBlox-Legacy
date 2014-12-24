#ifndef OB_INSTANCE_RUNSERVICE_H_
#define OB_INSTANCE_RUNSERVICE_H_

#include "Instance.h"

namespace ob_instance{
	class RunService: public Instance{
		public:
			RunService();
			virtual ~RunService();

			virtual void Destroy();

			virtual Instance* cloneImpl();

			virtual char* getClassName();

			virtual int wrap_lua(lua_State* L);

			ob_type::LuaEvent* RenderStepped;
			ob_type::LuaEvent* Stepped;
			ob_type::LuaEvent* Heartbeat;

			DECLARE_STATIC_INIT(RunService);
		protected:
			static char* ClassName;
			static char* LuaClassName;

			static void register_lua_events(lua_State* L);
	};
}
#endif
