#include "Enums.h"

namespace ob_enum{
	LuaEnum* LuaFormFactor = new LuaEnum("FormFactor", 4, "Symmetric", "Brick", "Plate", "Custom");

	LuaEnum* LuaCameraType = new LuaEnum("CameraType", 7, "Fixed", "Attach", "Watch", "Track", "Follow", "Custom", "Scriptable");
}
