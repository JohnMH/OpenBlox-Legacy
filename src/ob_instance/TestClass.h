#ifndef OB_INSTANCE_TESTCLASS_H_
#define OB_INSTANCE_TESTCLASS_H_

#include "Instance.h"

namespace ob_instance{
	class TestClass: public Instance{
		public:
			TestClass();
			virtual ~TestClass();

			virtual Instance* cloneImpl();

			virtual int wrap_lua(lua_State* L);

			DECLARE_STATIC_INIT(TestClass);
		protected:
			static char* ClassName;
			static char* LuaClassName;
	};
}
#endif
