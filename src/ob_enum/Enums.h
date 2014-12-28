#ifndef OB_ENUM_ENUMS_H_
#define OB_ENUM_ENUMS_H_

#include "LuaEnum.h"

namespace ob_enum{
	extern LuaEnum* LuaFormFactor;
	extern LuaEnum* LuaCameraType;

	enum class FormFactor{
		SYMMETRIC,
		BRICK,
		PLATE,
		CUSTOM
	};
	enum class CameraType{
		FIXED,
		ATTACH,
		WATCH,
		TRACK,
		FOLLOW,
		CUSTOM,
		SCRIPTABLE
	};

	void registerEnums(lua_State* L);
};
#endif
