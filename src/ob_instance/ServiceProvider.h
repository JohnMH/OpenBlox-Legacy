#ifndef OB_INSTANCE_SERVICEPROVIDER_H_
#define OB_INSTANCE_SERVICEPROVIDER_H_

#include "Instance.h"

namespace ob_instance{
	class ServiceProvider: public Instance{
		public:
			ServiceProvider();
			virtual ~ServiceProvider();

			virtual Instance* FindService(std::string className);
			virtual Instance* GetService(std::string className);

			virtual std::string getClassName();

			DECLARE_STATIC_INIT(ServiceProvider);

			static int lua_FindService(lua_State* L);
			static int lua_GetService(lua_State* L);
		protected:
			static std::string ClassName;
			static std::string LuaClassName;

			static void register_lua_methods(lua_State* L);
	};
}
#endif
