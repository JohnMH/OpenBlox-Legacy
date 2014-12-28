#ifndef OB_INSTANCE_DATAMODEL_H_
#define OB_INSTANCE_DATAMODEL_H_

#include "ServiceProvider.h"
#include "StarterGui.h"
#include "RunService.h"

namespace ob_instance{
	class DataModel: public ServiceProvider{
		public:
			DataModel();
			virtual ~DataModel();

			virtual void render();
			virtual Instance* GetService(std::string className);

			int wrap_lua(lua_State* L);

			virtual std::string getClassName();

			StarterGui* starterGui;
			RunService* runService;

			DECLARE_STATIC_INIT(DataModel);
		protected:
			static std::string ClassName;
			static std::string LuaClassName;

			static void register_lua_methods(lua_State* L);
			static void register_lua_property_getters(lua_State* L);
			static void register_lua_property_setters(lua_State* L);

			virtual Instance* cloneImpl();
	};
}
#endif
