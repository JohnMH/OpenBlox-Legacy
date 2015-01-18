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

			static int lua_FindService(lua_State* L);
			static int lua_GetService(lua_State* L);

			DECLARE_CLASS(ServiceProvider);

			static void register_lua_methods(lua_State* L);
	};
}
#endif
