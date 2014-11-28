#ifndef OB_INSTANCE_DATAMODEL_H_
#define OB_INSTANCE_DATAMODEL_H_

#include "Instance.h"

#include "../openblox/OpenBlox.h"

namespace ob_instance{
	class DataModel: public Instance{
		public:
			DataModel();
			virtual ~DataModel();

			DECLARE_STATIC_INIT(DataModel);

			virtual char* toString();

			int wrap_lua(lua_State *L);
		protected:
			static char* ClassName;

			virtual Instance* cloneImpl();
	};
}
#endif
