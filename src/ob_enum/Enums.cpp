#include "Enums.h"

namespace ob_enum{
	LuaEnum* LuaFormFactor = new LuaEnum("FormFactor", 4, "Symmetric", "Brick", "Plate", "Custom");

	LuaEnum* LuaCameraType = new LuaEnum("CameraType", 7, "Fixed", "Attach", "Watch", "Track", "Follow", "Custom", "Scriptable");

	void registerEnums(lua_State* L){
		lua_newtable(L);

		for(std::map<std::string, LuaEnum*>::iterator it = LuaEnum::Enums->begin(); it != LuaEnum::Enums->end(); ++it){
			LuaEnum* en = it->second;

			lua_pushstring(L, en->Type.c_str());
			en->wrap_lua(L);
			lua_rawset(L, -3);
		}

		lua_setglobal(L, "Enum");
	}
}
