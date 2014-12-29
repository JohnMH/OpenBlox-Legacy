#ifndef OB_ENUM_ENUMS_H_
#define OB_ENUM_ENUMS_H_

#include "LuaEnum.h"

namespace ob_enum{
	extern LuaEnum* LuaFormFactor;
	extern LuaEnum* LuaCameraType;

	enum class FormFactor{
		SYMMETRIC = 0,
		BRICK = 1,
		PLATE = 2,
		CUSTOM = 3
	};
	enum class CameraType{
		FIXED = 0,
		ATTACH = 1,
		WATCH = 2,
		TRACK = 3,
		FOLLOW = 4,
		CUSTOM = 5,
		SCRIPTABLE = 6
	};
	enum class HttpContentType{
		ApplicationJson = 0,
		ApplicationXml = 1,
		ApplicationUrlEncoded = 2,
		TextPlain = 3,
		TextXml = 4
	};

	void registerEnums(lua_State* L);
};
#endif
