#ifndef OPENBLOX_OPENBLOX_H_
#define OPENBLOX_OPENBLOX_H_

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>

#include <cstdlib>
#include <cstring>

extern "C"{
	#include "../lua/lua.h"
	#include "../lua/lualib.h"
	#include "../lua/lauxlib.h"
}

//Helpers
#include "static_init.h"
#include "Utility.h"
#include "Factory.h"

#include "BaseGame.h"
#endif
