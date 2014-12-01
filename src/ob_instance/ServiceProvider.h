#ifndef OB_INSTANCE_SERVICEPROVIDER_H_
#define OB_INSTANCE_SERVICEPROVIDER_H_

#include "Instance.h"

namespace ob_instance{
	class ServiceProvider: public Instance{
		public:
			ServiceProvider();
			virtual ~ServiceProvider();

			virtual Instance* FindService(const char* className);
			virtual Instance* GetService(const char* className);

			virtual char* getClassName();

			DECLARE_STATIC_INIT(ServiceProvider);

			static int lua_FindService(lua_State* L);
		protected:
			static char* ClassName;
			static char* LuaClassName;

			static void register_lua_methods(lua_State* L);
	};
}
#endif
