#include "Vector2.h"

namespace ob_type{
	STATIC_INIT(Vector2){
		lua_State* L = OpenBlox::BaseGame::getGlobalState();

		luaL_newmetatable(L, lua_vec2_name);

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

		lua_pushstring(L, "__gc");
		lua_pushcfunction(L, lua_gc);
		lua_rawset(L, -3);

		lua_pushstring(L, "__index");
		lua_pushcfunction(L, lua_index);
		lua_rawset(L, -3);

		lua_pushstring(L, "__newindex");
		lua_pushcfunction(L, lua_newindex);
		lua_rawset(L, -3);

		lua_pop(L, 1);
	}

	Vector2::Vector2(){
		x = 0;
		y = 0;
		magnitude = 1;

		usedInternally = false;
	}

	Vector2::Vector2(double x, double y){
		this->x = x;
		this->y = y;
		magnitude = sqrt(x * x + y * y);

		usedInternally = false;
	}

	Vector2::~Vector2(){}

	Vector2* Vector2::getNormalized(){
		Vector2* newGuy = new Vector2();
		newGuy->x = x;
		newGuy->y = y;
		return newGuy;
	}

	bool Vector2::equals(Vector2* other){
		if(other){
			return (x == other->x && y == other->y);
		}
		return false;
	}

	Vector2* Vector2::add(double v){
		return new Vector2(x + v, y + v);
	}

	Vector2* Vector2::add(Vector2* other){
		if(other == NULL){
			return NULL;
		}
		return new Vector2(x + other->x, y + other->y);
	}

	Vector2* Vector2::sub(double v){
		return new Vector2(x - v, y - v);
	}

	Vector2* Vector2::sub(Vector2* other){
		if(other == NULL){
			return NULL;
		}
		return new Vector2(x - other->x, y - other->y);
	}

	Vector2* Vector2::mul(double v){
		return new Vector2(x * v, y * v);
	}

	Vector2* Vector2::mul(Vector2* other){
		if(other == NULL){
			return NULL;
		}
		return new Vector2(x * other->x, y * other->y);
	}

	Vector2* Vector2::div(double v){
		return new Vector2(x / v, y / v);
	}

	Vector2* Vector2::div(Vector2* other){
		if(other == NULL){
			return NULL;
		}
		return new Vector2(x / other->x, y / other->y);
	}

	Vector2* Vector2::neg(){
		return new Vector2(-x, -y);
	}

	Vector2* Vector2::lerp(Vector2* goal, double alpha){
		if(goal == NULL){
			return NULL;
		}
		return add(alpha)->mul(goal->sub(this));
	}

	double Vector2::dot(Vector2* other){
		if(other == NULL){
			return NULL;
		}
		return x * other->x + y * other->y;
	}

	Vector2* Vector2::cross(Vector2* other){
		if(other == NULL){
			return NULL;
		}
		return new Vector2(x*other->y, y*other->y);//TODO: check this?
	}

	bool Vector2::isClose(Vector2* other, double epsilon){
		if(other == NULL){
			return NULL;
		}
		return sub(other)->magnitude <= epsilon;
	}

	int Vector2::lua_index(lua_State* L){
		Vector2* LuaVec2 = checkVector2(L, 1);
		if(LuaVec2 != NULL){
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

	int Vector2::lua_newindex(lua_State* L){
		const char* name = luaL_checkstring(L, 2);
		return luaL_error(L, "%s cannot be assigned to", name);
	}

	int Vector2::lua_toString(lua_State* L){
		Vector2* LuaVec2 = checkVector2(L, 1);
		std::string ret = "";
		ret += ((std::ostringstream&)(std::ostringstream() << std::dec << LuaVec2->x)).str() + ", " + ((std::ostringstream&)(std::ostringstream() << std::dec << LuaVec2->y)).str();
		lua_pushstring(L, ret.c_str());
		return 1;
	}

	int Vector2::lua_getUnit(lua_State* L){
		Vector2* LuaVec2 = checkVector2(L, 1);
		return LuaVec2->getNormalized()->wrap_lua(L);
	}

	int Vector2::lua_getMagnitude(lua_State* L){
		Vector2* LuaVec2 = checkVector2(L, 1);
		lua_pushnumber(L, LuaVec2->magnitude);
		return 1;
	}

	int Vector2::lua_getX(lua_State* L){
		Vector2* LuaVec2 = checkVector2(L, 1);
		lua_pushnumber(L, LuaVec2->x);
		return 1;
	}

	int Vector2::lua_getY(lua_State* L){
		Vector2* LuaVec2 = checkVector2(L, 1);
		lua_pushnumber(L, LuaVec2->y);
		return 1;
	}

	int Vector2::lua_lerp(lua_State* L){
		Vector2* LuaVec2 = checkVector2(L, 1);
		Vector2* GoalVec2 = checkVector2(L, 2);
		double alpha = luaL_checknumber(L, 3);
		return LuaVec2->lerp(GoalVec2, alpha)->wrap_lua(L);
	}

	int Vector2::lua_dot(lua_State* L){
		Vector2* LuaVec2 = checkVector2(L, 1);
		Vector2* OtherVec2 = checkVector2(L, 2);
		lua_pushnumber(L, LuaVec2->dot(OtherVec2));
		return 1;
	}

	int Vector2::lua_cross(lua_State* L){
		Vector2* LuaVec2 = checkVector2(L, 1);
		Vector2* OtherVec2 = checkVector2(L, 2);
		return LuaVec2->cross(OtherVec2)->wrap_lua(L);
	}

	int Vector2::lua_isClose(lua_State* L){
		Vector2* LuaVec2 = checkVector2(L, 1);
		Vector2* OtherVec2 = checkVector2(L, 2);
		double epsilon = 1e-6;
		if(lua_isnumber(L, 3)){
			epsilon = lua_tonumber(L, 3);
		}
		lua_pushboolean(L, LuaVec2->isClose(OtherVec2, epsilon));
		return 1;
	}

	int Vector2::lua_unm(lua_State* L){
		Vector2* LuaVec2 = checkVector2(L, 1);
		return LuaVec2->neg()->wrap_lua(L);
	}

	int Vector2::lua_add(lua_State* L){
		Vector2* LuaVec2 = checkVector2(L, 1);
		if(lua_isnumber(L, 2)){
			double numAdd = lua_tonumber(L, 2);
			return LuaVec2->add(numAdd)->wrap_lua(L);
		}else{
			Vector2* OtherVec2 = checkVector2(L, 2);
			return LuaVec2->add(OtherVec2)->wrap_lua(L);
		}
		return 0;
	}

	int Vector2::lua_sub(lua_State* L){
		Vector2* LuaVec2 = checkVector2(L, 1);
		if(lua_isnumber(L, 2)){
			double numSub = lua_tonumber(L, 2);
			return LuaVec2->sub(numSub)->wrap_lua(L);
		}else{
			Vector2* OtherVec2 = checkVector2(L, 2);
			return LuaVec2->sub(OtherVec2)->wrap_lua(L);
		}
		return 0;
	}

	int Vector2::lua_mul(lua_State* L){
		Vector2* LuaVec2 = checkVector2(L, 1);
		if(lua_isnumber(L, 2)){
			double numMul = lua_tonumber(L, 2);
			return LuaVec2->mul(numMul)->wrap_lua(L);
		}else{
			Vector2* OtherVec2 = checkVector2(L, 2);
			return LuaVec2->mul(OtherVec2)->wrap_lua(L);
		}
		return 0;
	}

	int Vector2::lua_div(lua_State* L){
		Vector2* LuaVec2 = checkVector2(L, 1);
		if(lua_isnumber(L, 2)){
			double numDiv = lua_tonumber(L, 2);
			return LuaVec2->div(numDiv)->wrap_lua(L);
		}else{
			Vector2* OtherVec2 = checkVector2(L, 2);
			return LuaVec2->div(OtherVec2)->wrap_lua(L);
		}
		return 0;
	}

	int Vector2::lua_eq(lua_State* L){
		Vector2* LuaVec2 = checkVector2(L, 1);
		if(lua_isuserdata(L, 2)){
			Vector2* OtherVec2 = checkVector2(L, 2);
			lua_pushboolean(L, LuaVec2->equals(OtherVec2));
		}else{
			lua_pushboolean(L, false);
		}
		return 1;
	}

	int Vector2::lua_gc(lua_State* L){
		void *p = lua_touserdata(L, 1);
		if(p){
			if(lua_getmetatable(L, 1)){
				lua_getfield(L, LUA_REGISTRYINDEX, lua_vec2_name);
				if(lua_rawequal(L, -1, -2)){
					lua_pop(L, 2);

					Vector2* vec_ud = *(Vector2**)p;
					delete vec_ud;
				}
			}
		}
		return 0;
	}

	int Vector2::wrap_lua(lua_State* L){
		Vector2** LuaVec2 = (Vector2**)lua_newuserdata(L, sizeof(*this));
		*LuaVec2 = this;

		luaL_getmetatable(L, lua_vec2_name);
		lua_setmetatable(L, -2);
		return 1;
	}

	Vector2* checkVector2(lua_State* L, int n){
		return *(Vector2**)luaL_checkudata(L, n, lua_vec2_name);
	}
}
