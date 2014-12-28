#include "Vector3.h"

namespace ob_type{
	STATIC_INIT(Vector3){
		lua_State* L = OpenBlox::BaseGame::getGlobalState();

		luaL_newmetatable(L, lua_vec3_name);

		lua_pushstring(L, "__metatable");
		lua_pushstring(L, "This metatable is locked");
		lua_rawset(L, -3);

		lua_pushstring(L, "__methods");
		lua_newtable(L);
		luaL_Reg methods[]{
			{"lerp", lua_lerp},
			{"Lerp", lua_lerp},
			{"dot", lua_dot},
			{"Dot", lua_dot},
			{"cross", lua_cross},
			{"Cross", lua_cross},
			{"isClose", lua_isClose},
			{"IsClose", lua_isClose},
			{NULL, NULL}
		};
		luaL_register(L, NULL, methods);
		lua_rawset(L, -3);

		lua_pushstring(L, "__properties");
		lua_newtable(L);
		luaL_Reg props[]{
			{"unit", lua_getUnit},
			{"Unit", lua_getUnit},
			{"magnitude", lua_getMagnitude},
			{"Magnitude", lua_getMagnitude},
			{"x", lua_getX},
			{"X", lua_getX},
			{"y", lua_getY},
			{"Y", lua_getY},
			{"z", lua_getZ},
			{"Z", lua_getZ},
			{NULL, NULL}
		};
		luaL_register(L, NULL, props);
		lua_rawset(L, -3);

		lua_pushstring(L, "__tostring");
		lua_pushcfunction(L, lua_toString);
		lua_rawset(L, -3);

		lua_pushstring(L, "__unm");
		lua_pushcfunction(L, lua_unm);
		lua_rawset(L, -3);

		lua_pushstring(L, "__add");
		lua_pushcfunction(L, lua_add);
		lua_rawset(L, -3);

		lua_pushstring(L, "__sub");
		lua_pushcfunction(L, lua_sub);
		lua_rawset(L, -3);

		lua_pushstring(L, "__mul");
		lua_pushcfunction(L, lua_mul);
		lua_rawset(L, -3);

		lua_pushstring(L, "__div");
		lua_pushcfunction(L, lua_div);
		lua_rawset(L, -3);

		lua_pushstring(L, "__eq");
		lua_pushcfunction(L, lua_eq);
		lua_rawset(L, -3);

		lua_pushstring(L, "__index");
		lua_pushcfunction(L, lua_index);
		lua_rawset(L, -3);

		lua_pushstring(L, "__newindex");
		lua_pushcfunction(L, lua_newindex);
		lua_rawset(L, -3);

		lua_pop(L, 1);
	}

	Vector3::Vector3(){
		x = 0;
		y = 0;
		z = 0;
		magnitude = 1;
	}

	Vector3::Vector3(double x, double y, double z){
		this->x = x;
		this->y = y;
		this->z = z;
		magnitude = sqrt(dot(this));
	}

	Vector3::~Vector3(){}

	Vector3* Vector3::getNormalized(){
		Vector3* newGuy = new Vector3();
		newGuy->x = x/magnitude;
		newGuy->y = y/magnitude;
		newGuy->z = z/magnitude;
		return newGuy;
	}

	bool Vector3::equals(Vector3* other){
		if(other){
			return (x == other->x && y == other->y && z == other->z);
		}
		return false;
	}

	Vector3* Vector3::add(double v){
		return new Vector3(x + v, y + v, z + v);
	}

	Vector3* Vector3::add(Vector3* other){
		if(other == NULL){
			return NULL;
		}
		return new Vector3(x + other->x, y + other->y, z + other->z);
	}

	Vector3* Vector3::sub(double v){
		return new Vector3(x - v, y - v, z - v);
	}

	Vector3* Vector3::sub(Vector3* other){
		if(other == NULL){
			return NULL;
		}
		return new Vector3(x - other->x, y - other->y, z - other->z);
	}

	Vector3* Vector3::mul(double v){
		return new Vector3(x * v, y * v, z * v);
	}

	Vector3* Vector3::mul(Vector3* other){
		if(other == NULL){
			return NULL;
		}
		return new Vector3(x * other->x, y * other->y, z * other->z);
	}

	Vector3* Vector3::div(double v){
		return new Vector3(x / v, y / v, z / v);
	}

	Vector3* Vector3::div(Vector3* other){
		if(other == NULL){
			return NULL;
		}
		return new Vector3(x / other->x, y / other->y, z / other->z);
	}

	Vector3* Vector3::neg(){
		return new Vector3(-x, -y, -z);
	}

	Vector3* Vector3::lerp(Vector3* goal, double alpha){
		if(goal == NULL){
			return NULL;
		}
		return add(alpha)->mul(goal->sub(this));
	}

	double Vector3::dot(Vector3* other){
		if(other == NULL){
			return 0;
		}
		return x*other->x + y*other->y + z*other->z;
	}

	Vector3* Vector3::cross(Vector3* other){
		if(other == NULL){
			return NULL;
		}
		return new Vector3(y*other->z - z*other->y, x*other->z - other->z*x, x*other->y - other->y*x);
	}

	bool Vector3::isClose(Vector3* other, double epsilon){
		if(other == NULL){
			return NULL;
		}
		return sub(other)->magnitude <= epsilon;
	}

	int Vector3::lua_index(lua_State* L){
		Vector3* LuaVec3 = checkVector3(L, 1);
		if(LuaVec3 != NULL){
			const char* name = luaL_checkstring(L, 2);

			lua_getmetatable(L, 1);
			lua_getfield(L, -1, "__properties");
			lua_getfield(L, -1, name);
			if(lua_iscfunction(L, -1)){
				lua_remove(L, -2);
				lua_remove(L, -2);

				lua_pushvalue(L, 1);
				lua_call(L, 1, 1);
				return 1;
			}else{
				lua_pop(L, 2);
				//Check methods
				lua_getfield(L, -1, "__methods");//-2
				lua_getfield(L, -1, name);//-1
				if(lua_iscfunction(L, -1)){
					lua_remove(L, -2);
					lua_remove(L, -3);

					return 1;
				}else{
					lua_pop(L, 3);
					return luaL_error(L, "attempt to index '%s' (a nil value)", name);
				}
			}
		}
		return 0;
	}

	int Vector3::lua_newindex(lua_State* L){
		const char* name = luaL_checkstring(L, 2);
		return luaL_error(L, "%s cannot be assigned to", name);
	}

	int Vector3::lua_toString(lua_State* L){
		Vector3* LuaVec3 = checkVector3(L, 1);
		std::string ret = "";
		ret += ((std::ostringstream&)(std::ostringstream() << std::dec << LuaVec3->x)).str() + ", " + ((std::ostringstream&)(std::ostringstream() << std::dec << LuaVec3->y)).str() + ", " + ((std::ostringstream&)(std::ostringstream() << std::dec << LuaVec3->z)).str();
		lua_pushstring(L, ret.c_str());
		return 1;
	}

	int Vector3::lua_getUnit(lua_State* L){
		Vector3* LuaVec3 = checkVector3(L, 1);
		return LuaVec3->getNormalized()->wrap_lua(L);
	}

	int Vector3::lua_getMagnitude(lua_State* L){
		Vector3* LuaVec3 = checkVector3(L, 1);
		lua_pushnumber(L, LuaVec3->magnitude);
		return 1;
	}

	int Vector3::lua_getX(lua_State* L){
		Vector3* LuaVec3 = checkVector3(L, 1);
		lua_pushnumber(L, LuaVec3->x);
		return 1;
	}

	int Vector3::lua_getY(lua_State* L){
		Vector3* LuaVec3 = checkVector3(L, 1);
		lua_pushnumber(L, LuaVec3->y);
		return 1;
	}

	int Vector3::lua_getZ(lua_State* L){
		Vector3* LuaVec3 = checkVector3(L, 1);
		lua_pushnumber(L, LuaVec3->z);
		return 1;
	}

	int Vector3::lua_lerp(lua_State* L){
		Vector3* LuaVec3 = checkVector3(L, 1);
		Vector3* GoalVec3 = checkVector3(L, 2);
		double alpha = luaL_checknumber(L, 3);
		return LuaVec3->lerp(GoalVec3, alpha)->wrap_lua(L);
	}

	int Vector3::lua_dot(lua_State* L){
		Vector3* LuaVec3 = checkVector3(L, 1);
		Vector3* OtherVec3 = checkVector3(L, 2);
		lua_pushnumber(L, LuaVec3->dot(OtherVec3));
		return 1;
	}

	int Vector3::lua_cross(lua_State* L){
		Vector3* LuaVec3 = checkVector3(L, 1);
		Vector3* OtherVec3 = checkVector3(L, 2);
		return LuaVec3->cross(OtherVec3)->wrap_lua(L);
	}

	int Vector3::lua_isClose(lua_State* L){
		Vector3* LuaVec3 = checkVector3(L, 1);
		Vector3* OtherVec3 = checkVector3(L, 2);
		double epsilon = 1e-6;
		if(lua_isnumber(L, 3)){
			epsilon = lua_tonumber(L, 3);
		}
		lua_pushboolean(L, LuaVec3->isClose(OtherVec3, epsilon));
		return 1;
	}

	int Vector3::lua_unm(lua_State* L){
		Vector3* LuaVec3 = checkVector3(L, 1);
		return LuaVec3->neg()->wrap_lua(L);
	}

	int Vector3::lua_add(lua_State* L){
		Vector3* LuaVec3 = checkVector3(L, 1);
		if(lua_isnumber(L, 2)){
			double numAdd = lua_tonumber(L, 2);
			return LuaVec3->add(numAdd)->wrap_lua(L);
		}else{
			Vector3* OtherVec3 = checkVector3(L, 2);
			return LuaVec3->add(OtherVec3)->wrap_lua(L);
		}
		return 0;
	}

	int Vector3::lua_sub(lua_State* L){
		Vector3* LuaVec3 = checkVector3(L, 1);
		if(lua_isnumber(L, 2)){
			double numSub = lua_tonumber(L, 2);
			return LuaVec3->sub(numSub)->wrap_lua(L);
		}else{
			Vector3* OtherVec3 = checkVector3(L, 2);
			return LuaVec3->sub(OtherVec3)->wrap_lua(L);
		}
		return 0;
	}

	int Vector3::lua_mul(lua_State* L){
		Vector3* LuaVec3 = checkVector3(L, 1);
		if(lua_isnumber(L, 2)){
			double numMul = lua_tonumber(L, 2);
			return LuaVec3->mul(numMul)->wrap_lua(L);
		}else{
			Vector3* OtherVec3 = checkVector3(L, 2);
			return LuaVec3->mul(OtherVec3)->wrap_lua(L);
		}
		return 0;
	}

	int Vector3::lua_div(lua_State* L){
		Vector3* LuaVec3 = checkVector3(L, 1);
		if(lua_isnumber(L, 2)){
			double numDiv = lua_tonumber(L, 2);
			return LuaVec3->div(numDiv)->wrap_lua(L);
		}else{
			Vector3* OtherVec3 = checkVector3(L, 2);
			return LuaVec3->div(OtherVec3)->wrap_lua(L);
		}
		return 0;
	}

	int Vector3::lua_eq(lua_State* L){
		Vector3* LuaVec3 = checkVector3(L, 1);
		if(lua_isuserdata(L, 2)){
			Vector3* OtherVec3 = checkVector3(L, 2);
			lua_pushboolean(L, LuaVec3->equals(OtherVec3));
		}else{
			lua_pushboolean(L, false);
		}
		return 1;
	}

	int Vector3::wrap_lua(lua_State* L){
		Vector3** LuaVec3 = (Vector3**)lua_newuserdata(L, sizeof(*this));
		*LuaVec3 = this;

		luaL_getmetatable(L, lua_vec3_name);
		lua_setmetatable(L, -2);
		return 1;
	}

	Vector3* checkVector3(lua_State* L, int n){
		return *(Vector3**)luaL_checkudata(L, n, lua_vec3_name);
	}
}
