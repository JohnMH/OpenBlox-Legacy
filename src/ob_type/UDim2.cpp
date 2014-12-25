#include "UDim2.h"

namespace ob_type{
	STATIC_INIT(UDim2){
		lua_State* L = OpenBlox::BaseGame::getGlobalState();

		luaL_newmetatable(L, lua_udim2_name);

		lua_pushstring(L, "__metatable");
		lua_pushstring(L, "This metatable is locked");
		lua_rawset(L, -3);

		lua_pushstring(L, "__methods");
		lua_newtable(L);
		luaL_Reg methods[]{
			{NULL, NULL}
		};
		luaL_register(L, NULL, methods);
		lua_rawset(L, -3);

		lua_pushstring(L, "__properties");
		lua_newtable(L);
		luaL_Reg props[]{
			{"X", lua_getX},
			{"x", lua_getX},
			{"Y", lua_getY},
			{"y", lua_getY},
			{NULL, NULL}
		};
		luaL_register(L, NULL, props);
		lua_rawset(L, -3);

		lua_pushstring(L, "__tostring");
		lua_pushcfunction(L, lua_toString);
		lua_rawset(L, -3);

		lua_pushstring(L, "__add");
		lua_pushcfunction(L, lua_add);
		lua_rawset(L, -3);

		lua_pushstring(L, "__sub");
		lua_pushcfunction(L, lua_sub);
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

	UDim2::UDim2(){
		x = new UDim(0, 0);
		y = new UDim(0, 0);
	}

	UDim2::UDim2(double xscale, int xoffset, double yscale, int yoffset){
		x = new UDim(xscale, xoffset);
		y = new UDim(yscale, yoffset);
	}

	UDim2::~UDim2(){
		delete x;
		delete y;
	}

	bool UDim2::equals(UDim2* other){
		if(other){
			return (x->equals(other->x) && y->equals(other->y));
		}
		return false;
	}

	UDim2* UDim2::add(UDim2* other){
		if(other){
			return new UDim2(x->scale + other->x->scale, x->offset + other->x->offset, y->scale + other->y->scale, y->offset + other->y->offset);
		}
		return NULL;
	}

	UDim2* UDim2::sub(UDim2* other){
		if(other){
			return new UDim2(x->scale - other->x->scale, x->offset - other->x->offset, y->scale - other->y->scale, y->offset - other->y->offset);
		}
		return NULL;
	}

	UDim2* UDim2::clone(){
		UDim2* newGuy = new UDim2();
		delete newGuy->x;
		delete newGuy->y;
		newGuy->x = x->clone();
		newGuy->y = y->clone();

		return newGuy;
	}

	int UDim2::lua_index(lua_State* L){
		UDim2* LuaUDim2 = checkUDim2(L, 1);
		if(LuaUDim2 != NULL){
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

	int UDim2::lua_newindex(lua_State* L){
		const char* name = luaL_checkstring(L, 2);
		return luaL_error(L, "%s cannot be assigned to", name);
	}

	int UDim2::lua_toString(lua_State* L){
		UDim2* LuaUDim2 = checkUDim2(L, 1);
		std::string ret = "";
		ret += "{" + ((std::ostringstream&)(std::ostringstream() << std::dec << LuaUDim2->x->scale)).str() + ", " + ((std::ostringstream&)(std::ostringstream() << std::dec << LuaUDim2->x->offset)).str() + "}, {"+ ((std::ostringstream&)(std::ostringstream() << std::dec << LuaUDim2->y->scale)).str() + ", " + ((std::ostringstream&)(std::ostringstream() << std::dec << LuaUDim2->y->offset)).str() + "}";
		lua_pushstring(L, ret.c_str());
		return 1;
	}

	int UDim2::lua_getX(lua_State* L){
		UDim2* LuaUDim2 = checkUDim2(L, 1);
		return LuaUDim2->x->clone()->wrap_lua(L);
	}

	int UDim2::lua_getY(lua_State* L){
		UDim2* LuaUDim2 = checkUDim2(L, 1);
		return LuaUDim2->y->clone()->wrap_lua(L);
	}

	int UDim2::lua_add(lua_State* L){
		UDim2* LuaUDim2 = checkUDim2(L, 1);
		UDim2* OtherUDim2 = checkUDim2(L, 2);

		return LuaUDim2->add(OtherUDim2)->wrap_lua(L);
	}

	int UDim2::lua_sub(lua_State* L){
		UDim2* LuaUDim2 = checkUDim2(L, 1);
		UDim2* OtherUDim2 = checkUDim2(L, 2);

		return LuaUDim2->sub(OtherUDim2)->wrap_lua(L);
	}

	int UDim2::lua_eq(lua_State* L){
		UDim2* LuaUDim2 = checkUDim2(L, 1);
		if(lua_isuserdata(L, 2)){
			UDim2* OtherUDim2 = checkUDim2(L, 2);
			lua_pushboolean(L, LuaUDim2->equals(OtherUDim2));
		}else{
			lua_pushboolean(L, false);
		}
		return 1;
	}

	int UDim2::wrap_lua(lua_State* L){
		UDim2** LuaUDim2 = (UDim2**)lua_newuserdata(L, sizeof(*this));
		*LuaUDim2 = this;

		luaL_getmetatable(L, lua_udim2_name);
		lua_setmetatable(L, -2);
		return 1;
	}

	UDim2* checkUDim2(lua_State* L, int n){
		return *(UDim2**)luaL_checkudata(L, n, lua_udim2_name);
	}
}
