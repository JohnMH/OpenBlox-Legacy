#include "LuaEnum.h"
#include <map>

namespace ob_enum{
	//Util
	LuaEnumItem* checkEnumItem(lua_State* L, int n){
		return *(LuaEnumItem**)luaL_checkudata(L, n, lua_enumitem_name);
	}

	LuaEnum* checkEnum(lua_State* L, int n){
		return *(LuaEnum**)luaL_checkudata(L, n, lua_enum_name);
	}

	std::map<std::string, LuaEnum*>* LuaEnum::Enums = NULL;

	//LuaEnums
	LuaEnum::LuaEnum(std::string type, int numValues, ...){
		Type = type;
		EnumValues = std::map<std::string, LuaEnumItem*>();
		va_list args;
		va_start(args, numValues);
		for(int i=0; i<numValues; i++){
			const char* arg = va_arg(args, const char*);
			EnumValues[arg] = new LuaEnumItem(type, arg, i);
		}
		va_end(args);

		if(Enums == NULL){
			Enums = new std::map<std::string, LuaEnum*>();
		}

		Enums->insert(std::pair<std::string, ob_enum::LuaEnum*>(type, this));
	}

	LuaEnum::~LuaEnum(){}

	STATIC_INIT(LuaEnum){
		lua_State* L = OpenBlox::BaseGame::getGlobalState();

		luaL_newmetatable(L, lua_enum_name);

		lua_pushstring(L, "__metatable");
		lua_pushstring(L, "This metatable is locked");
		lua_rawset(L, -3);

		lua_pushstring(L, "__index");
		lua_pushcfunction(L, LuaEnum::lua_index);
		lua_rawset(L, -3);

		lua_pushstring(L, "__tostring");
		lua_pushcfunction(L, LuaEnum::lua_toString);
		lua_rawset(L, -3);

		lua_pop(L, 1);
	}

	LuaEnumItem* LuaEnum::GetEnumItem(int value){
		for(std::map<std::string, LuaEnumItem*>::iterator it = EnumValues.begin(); it != EnumValues.end(); ++it){
			if(it->second->Value == value){
				return it->second;
			}
		}
		return NULL;
	}

	int LuaEnum::wrap_lua(lua_State* L){
		LuaEnum** enm = (LuaEnum**)lua_newuserdata(L, sizeof(*this));
		*enm = this;

		luaL_getmetatable(L, lua_enum_name);
		lua_setmetatable(L, -2);

		return 1;
	}

	int LuaEnum::lua_index(lua_State* L){
		LuaEnum* con = checkEnum(L, 1);
		if(con != NULL){
			const char* name = luaL_checkstring(L, 2);
			//TODO: make this work
			LuaEnumItem* enm = con->EnumValues[std::string(name)];
			if (enm != NULL) {
				return enm->wrap_lua(L);
			}
		}
		return 0;
	}

	int LuaEnum::lua_toString(lua_State* L){
		LuaEnum* itm = checkEnum(L, 1);
		std::string name = "Enum.";
		name += itm->Type;
		lua_pushstring(L, name.c_str());
		return 1;
	}

	//EnumItems
	LuaEnumItem::LuaEnumItem(std::string type, const char* name, int value){
		Type = type;
		Name = name;
		Value = value;
	}

	LuaEnumItem::~LuaEnumItem(){}

	STATIC_INIT(LuaEnumItem){
		lua_State* L = OpenBlox::BaseGame::getGlobalState();

		luaL_newmetatable(L, lua_enumitem_name);

		lua_pushstring(L, "__metatable");
		lua_pushstring(L, "This metatable is locked");
		lua_rawset(L, -3);

		lua_pushstring(L, "__index");
		lua_pushcfunction(L, LuaEnumItem::lua_index);
		lua_rawset(L, -3);

		lua_pushstring(L, "__tostring");
		lua_pushcfunction(L, LuaEnumItem::lua_toString);
		lua_rawset(L, -3);

		lua_pop(L, 1);
	}

	template<class T> T LuaEnumItem::ToEnum(){
		return (T)(this->Value);
	}

	int LuaEnumItem::wrap_lua(lua_State* L){
		LuaEnumItem** enm = (LuaEnumItem**)lua_newuserdata(L, sizeof(*this));
		*enm = this;

		luaL_getmetatable(L, lua_enumitem_name);
		lua_setmetatable(L, -2);

		return 1;
	}

	int LuaEnumItem::lua_index(lua_State* L){
		LuaEnumItem* itm = checkEnumItem(L, 1);
		std::string propname = std::string(luaL_checkstring(L, 2));

		// there's only two items to an EnumItem, so only return those two
		if(propname == "Name"){
			lua_pushstring(L, itm->Name.c_str());
			return 1;
		}
		if(propname == "Value"){
			lua_pushinteger(L, itm->Value);
			return 1;
		}
		return luaL_error(L, "attempt to index '%s' (a nil value)", propname.c_str());
	}

	int LuaEnumItem::lua_toString(lua_State* L){
		LuaEnumItem* itm = checkEnumItem(L, 1);
		std::string name = "Enum.";
		name += itm->Type;
		name += ".";
		name += itm->Name;
		lua_pushstring(L, name.c_str());
		return 1;
	}
}
