#ifndef OB_ENUM_LUAENUM_H_
#define OB_ENUM_LUAENUM_H_

#include "../openblox/OpenBlox.h"
#define lua_enum_name "luaL_Enum"
#define lua_enumitem_name "luaL_EnumItem"

namespace ob_enum{
	class LuaEnumItem{
		public:
			LuaEnumItem(std::string type, const char* name, int value);
			virtual ~LuaEnumItem();

			std::string Type;
			std::string Name;
			int Value;

			template<class T> T ToEnum();

			int wrap_lua(lua_State* L);
			static int lua_index(lua_State* L);
			static int lua_toString(lua_State* L);

			DECLARE_STATIC_INIT(LuaEnumItem);
	};

	class LuaEnum{
		public:
			LuaEnum(std::string type, int numValues, ...);
			virtual ~LuaEnum();

			std::map<std::string, LuaEnumItem*> EnumValues;
			std::string Type;
			LuaEnumItem* GetEnumItem(int value);

			int wrap_lua(lua_State* L);
			static int lua_index(lua_State* L);
			static int lua_toString(lua_State* L);

			static std::map<std::string, LuaEnum*>* Enums;

			DECLARE_STATIC_INIT(LuaEnum);
	};

	LuaEnumItem* checkEnumItem(lua_State* L, int idx, LuaEnum* enum_type);
	LuaEnum* checkEnum(lua_State* L, int idx);
};
#endif
