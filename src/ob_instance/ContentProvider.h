#ifndef OB_INSTANCE_CONTENTPROVIDER_H_
#define OB_INSTANCE_CONTENTPROVIDER_H_

#include "Instance.h"

namespace ob_instance{
	class ContentProvider: public Instance{
		public:
		ContentProvider();
			virtual ~ContentProvider();

			void Preload(std::string url);

			DECLARE_CLASS(ContentProvider); //Starts protected block

			static void register_lua_methods(lua_State* L);
	};
}
#endif
