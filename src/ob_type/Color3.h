#ifndef OB_TYPE_COLOR3_H_
#define OB_TYPE_COLOR3_H_

#include "../openblox/OpenBlox.h"

#define lua_color3_name "luaL_Color3"

namespace ob_type{
	class Color3{
		public:
			Color3();
			Color3(double r, double g, double b);
			virtual ~Color3();

			DECLARE_STATIC_INIT(Color3);

			double r;
			double g;
			double b;

			bool equals(Color3* other);

			Color3* clone();

			static int lua_index(lua_State* L);
			static int lua_newindex(lua_State* L);

			static int lua_toString(lua_State* L);

			static int lua_getR(lua_State* L);
			static int lua_getG(lua_State* L);
			static int lua_getB(lua_State* L);

			static int lua_eq(lua_State* L);

			int wrap_lua(lua_State* L);
	};

	Color3* checkColor3(lua_State* L, int n);
}
#endif
