#ifndef OB_TYPE_UDIM2_H_
#define OB_TYPE_UDIM2_H_

#include "../openblox/OpenBlox.h"

#include "UDim.h"

#define lua_udim2_name "luaL_UDim2"

namespace ob_type{
	class UDim2{
		public:
			UDim2(double xscale, int xoffset, double yscale, int yoffset);
			virtual ~UDim2();

			DECLARE_STATIC_INIT(UDim2);

			UDim* x;
			UDim* y;

			bool equals(UDim2* other);

			UDim2* clone();

			UDim2* add(UDim2* other);
			UDim2* sub(UDim2* other);

			static int lua_index(lua_State* L);
			static int lua_newindex(lua_State* L);

			static int lua_toString(lua_State* L);

			static int lua_getX(lua_State* L);
			static int lua_getY(lua_State* L);

			static int lua_add(lua_State* L);
			static int lua_sub(lua_State* L);
			static int lua_eq(lua_State* L);

			int wrap_lua(lua_State* L);
		private:
			UDim2();
	};

	UDim2* checkUDim2(lua_State* L, int n);
}
#endif
