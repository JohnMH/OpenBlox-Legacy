#ifndef OB_TYPE_UDIM_H_
#define OB_TYPE_UDIM_H_

#include "../openblox/OpenBlox.h"

#define lua_udim_name "luaL_UDim"

namespace ob_type{
	class UDim{
		public:
			UDim(double scale, int offset);
			virtual ~UDim();

			DECLARE_STATIC_INIT(UDim);

			double scale;
			int offset;

			bool equals(UDim* other);

			UDim* clone();

			static int lua_index(lua_State* L);
			static int lua_newindex(lua_State* L);

			static int lua_toString(lua_State* L);

			static int lua_getScale(lua_State* L);
			static int lua_getOffset(lua_State* L);

			static int lua_eq(lua_State* L);

			int wrap_lua(lua_State* L);
		private:
			UDim();
	};

	UDim* checkUDim(lua_State* L, int n);
}
#endif
