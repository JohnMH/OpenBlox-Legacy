#ifndef OB_INSTANCE_INSTANCE_H_
#define OB_INSTANCE_INSTANCE_H_

#include "../openblox/OpenBlox.h"

namespace ob_instance{
	class Instance{
		public:
			Instance();
			virtual ~Instance();

			DECLARE_STATIC_INIT(Instance);
	};
}
#endif
