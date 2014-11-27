#ifndef OB_INSTANCE_DATAMODEL_H_
#define OB_INSTANCE_DATAMODEL_H_

#include "Instances.h"

namespace ob_instance{
	class DataModel: public Instance{
		public:
			DataModel();
			virtual ~DataModel();

			DECLARE_STATIC_INIT(DataModel);

			void wrap_lua_impl(lua_State *L);
	};
}
#endif
