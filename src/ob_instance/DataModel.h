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

			StarterGui* starterGui;
			RunService* runService;

			DECLARE_CLASS(DataModel);

			static void register_lua_methods(lua_State* L);
			static void register_lua_property_getters(lua_State* L);
			static void register_lua_property_setters(lua_State* L);
	};
}
#endif
