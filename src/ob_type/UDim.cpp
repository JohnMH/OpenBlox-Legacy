#include "UDim.h"

namespace ob_type{
	STATIC_INIT(UDim){
		lua_State* L = OpenBlox::BaseGame::getGlobalState();

		luaL_newmetatable(L, lua_udim_name);

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
			{"Scale", lua_getScale},
			{"scale", lua_getScale},
			{"Offset", lua_getOffset},
			{"offset", lua_getOffset},
			{NULL, NULL}
		};
		luaL_register(L, NULL, props);
		lua_rawset(L, -3);

		lua_pushstring(L, "__tostring");
		lua_pushcfunction(L, lua_toString);
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

	UDim::UDim(){
		scale = 0;
		offset = 0;
	}

	UDim::UDim(double scale, int offset){
		this->scale = scale;
		this->offset = offset;
	}

	UDim::~UDim(){}

	bool UDim::equals(UDim* other){
		if(other){
			return (scale == other->scale && offset == other->offset);
		}
		return false;
	}

	UDim* UDim::clone(){
		UDim* newGuy = new UDim();
		newGuy->scale = scale;
		newGuy->offset = offset;

		return newGuy;
	}

	int UDim::lua_index(lua_State* L){
		UDim* LuaUDim = checkUDim(L, 1);
		if(LuaUDim != NULL){
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

	int UDim::lua_newindex(lua_State* L){
		const char* name = luaL_checkstring(L, 2);
		return luaL_error(L, "%s cannot be assigned to", name);
	}

	int UDim::lua_toString(lua_State* L){
		UDim* LuaUDim = checkUDim(L, 1);
		std::string ret = "";
		ret += ((std::ostringstream&)(std::ostringstream() << std::dec << LuaUDim->scale)).str() + ", " + ((std::ostringstream&)(std::ostringstream() << std::dec << LuaUDim->offset)).str();
		lua_pushstring(L, ret.c_str());
		return 1;
	}

	int UDim::lua_getScale(lua_State* L){
		UDim* LuaUDim = checkUDim(L, 1);
		lua_pushnumber(L, LuaUDim->scale);
		return 1;
	}

	int UDim::lua_getOffset(lua_State* L){
		UDim* LuaUDim = checkUDim(L, 1);
		lua_pushnumber(L, LuaUDim->offset);
		return 1;
	}

	int UDim::lua_eq(lua_State* L){
		UDim* LuaUDim = checkUDim(L, 1);
		if(lua_isuserdata(L, 2)){
			UDim* OtherUDim = checkUDim(L, 2);
			lua_pushboolean(L, LuaUDim->equals(OtherUDim));
		}else{
			lua_pushboolean(L, false);
		}
		return 1;
	}

	int UDim::wrap_lua(lua_State* L){
		UDim** LuaUDim = (UDim**)lua_newuserdata(L, sizeof(*this));
		*LuaUDim = this;

		luaL_getmetatable(L, lua_udim_name);
		lua_setmetatable(L, -2);
		return 1;
	}

	UDim* checkUDim(lua_State* L, int n){
		return *(UDim**)luaL_checkudata(L, n, lua_udim_name);
	}
}
