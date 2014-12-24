#ifndef OB_TYPE_VECTOR3_H_
#define OB_TYPE_VECTOR3_H_

#include "../openblox/OpenBlox.h"
#include <cmath>

#define lua_vec3_name "luaL_Vector3"

namespace ob_type{
	class Vector3{
		public:
			Vector3();
			Vector3(double x, double y, double z);
			virtual ~Vector3();

			DECLARE_STATIC_INIT(Vector3);

			bool usedInternally;

			double x;
			double y;
			double z;
			double magnitude;

			Vector3* getNormalized();//unit

			bool equals(Vector3* other);

			Vector3* add(double v);
			Vector3* add(Vector3* other);
			Vector3* sub(double v);
			Vector3* sub(Vector3* other);
			Vector3* mul(double v);
			Vector3* mul(Vector3* other);
			Vector3* div(double v);
			Vector3* div(Vector3* other);
			Vector3* neg();

			Vector3* lerp(Vector3* goal, double alpha);
			double dot(Vector3* other);
			Vector3* cross(Vector3* other);
			bool isClose(Vector3* other, double epsilon = 1e-6);

			static int lua_index(lua_State* L);
			static int lua_newindex(lua_State* L);

			static int lua_toString(lua_State* L);
			static int lua_getUnit(lua_State* L);
			static int lua_getMagnitude(lua_State* L);
			static int lua_getX(lua_State* L);
			static int lua_getY(lua_State* L);
			static int lua_getZ(lua_State* L);
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

	Vector3* checkVector3(lua_State* L, int n);
}
#endif
