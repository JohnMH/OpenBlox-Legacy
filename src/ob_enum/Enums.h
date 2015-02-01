#ifndef OB_ENUM_ENUMS_H_
#define OB_ENUM_ENUMS_H_

#include "LuaEnum.h"

namespace ob_enum{
	#define LENUM(typeName) extern LuaEnum* Lua##typeName; \
	enum class typeName

	LENUM(FormFactor){
		SYMMETRIC = 0,
		BRICK = 1,
		PLATE = 2,
		CUSTOM = 3
	};

	LENUM(CameraType){
		FIXED = 0,
		ATTACH = 1,
		WATCH = 2,
		TRACK = 3,
		FOLLOW = 4,
		CUSTOM = 5,
		SCRIPTABLE = 6
	};

	LENUM(HttpContentType){
		ApplicationJson = 0,
		ApplicationXml = 1,
		ApplicationUrlEncoded = 2,
		TextPlain = 3,
		TextXml = 4
	};

	LENUM(MouseButton){
		Left = 0,
		Right = 1,
		Middle = 2
	};

	void registerEnums(lua_State* L);
};
#endif
