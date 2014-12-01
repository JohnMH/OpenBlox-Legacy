#ifndef OB_INSTANCE_DATAMODEL_H_
#define OB_INSTANCE_DATAMODEL_H_

#include "ServiceProvider.h"

namespace ob_instance{
	class DataModel: public ServiceProvider{
		public:
			DataModel();
			virtual ~DataModel();

			virtual void render();

			int wrap_lua(lua_State* L);

			virtual char* getClassName();

			DECLARE_STATIC_INIT(DataModel);

		protected:
			static char* ClassName;
			static char* LuaClassName;

			static void register_lua_methods(lua_State* L);
			static void register_lua_property_getters(lua_State* L);
			static void register_lua_property_setters(lua_State* L);

			virtual Instance* cloneImpl();
	};
}
#endif
