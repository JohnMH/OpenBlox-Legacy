#ifndef OB_TYPE_VECTOR2_H_
#define OB_TYPE_VECTOR2_H_

#include "../openblox/OpenBlox.h"
#include <cmath>

#define lua_vec2_name "luaL_Vector2"

namespace ob_type{
	class Vector2{
		public:
			Vector2();
			Vector2(double x, double y);
			virtual ~Vector2();

			DECLARE_STATIC_INIT(Vector2);

			double x;
			double y;
			double magnitude;

			Vector2* getNormalized();//unit

			bool equals(Vector2* other);

			Vector2* clone();

			Vector2* add(double v);
			Vector2* add(Vector2* other);
			Vector2* sub(double v);
			Vector2* sub(Vector2* other);
			Vector2* mul(double v);
			Vector2* mul(Vector2* other);
			Vector2* div(double v);
			Vector2* div(Vector2* other);
			Vector2* neg();

			Vector2* lerp(Vector2* goal, double alpha);
			double dot(Vector2* other);
			Vector2* cross(Vector2* other);
			bool isClose(Vector2* other, double epsilon = 1e-6);

			static int lua_index(lua_State* L);
			static int lua_newindex(lua_State* L);

			static int lua_toString(lua_State* L);
			static int lua_getUnit(lua_State* L);
			static int lua_getMagnitude(lua_State* L);
			static int lua_getX(lua_State* L);
			static int lua_getY(lua_State* L);
			static int lua_lerp(lua_State* L);
			static int lua_dot(lua_State* L);
			static int lua_cross(lua_State* L);
			static int lua_isClose(lua_State* L);

			static int lua_unm(lua_State* L);
			static int lua_add(lua_State* L);
			static int lua_sub(lua_State* L);
			static int lua_mul(lua_State* L);
			static int lua_div(lua_State* L);
			static int lua_eq(lua_State* L);

			int wrap_lua(lua_State* L);
	};

	Vector2* checkVector2(lua_State* L, int n);
}
#endif
