#ifndef OB_ENUM_LUAENUM_H_
#define OB_ENUM_LUAENUM_H_

#include "../openblox/OpenBlox.h"
#define lua_enum_name "luaL_Enum"
#define lua_enumitem_name "luaL_EnumItem"

namespace ob_enum{
	class LuaEnumItem{
		public:
			LuaEnumItem(const char* type, const char* name, int value);
			DECLARE_STATIC_INIT(LuaEnumItem);
			virtual ~LuaEnumItem();
			const char* Type;
			const char* Name;
			int Value;
			template<class T> T ToEnum();

			int wrap_lua(lua_State* L);
			static int lua_index(lua_State* L);
			static int lua_toString(lua_State* L);
	};

	class LuaEnum{
		public:
			LuaEnum(const char* type, int numValues, ...);
			DECLARE_STATIC_INIT(LuaEnum);
			virtual ~LuaEnum();
			const char* Type;
			LuaEnumItem* GetEnumItem(int value);

			int wrap_lua(lua_State* L);
			static int lua_index(lua_State* L);
			static int lua_toString(lua_State* L);
		protected:
			std::map<std::string, LuaEnumItem*> EnumValues;
	};

	LuaEnumItem* checkEnumItem(lua_State* L, int idx);
	LuaEnum* checkEnum(lua_State* L, int idx);
};
#endif
