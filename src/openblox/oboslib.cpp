#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "oboslib.h"

static int getboolfield (lua_State *L, const char *key) {
  int res;
  lua_getfield(L, -1, key);
  res = lua_isnil(L, -1) ? -1 : lua_toboolean(L, -1);
  lua_pop(L, 1);
  return res;
}


static int getfield (lua_State *L, const char *key, int d) {
  int res;
  lua_getfield(L, -1, key);
  if (lua_isnumber(L, -1))
    res = (int)lua_tointeger(L, -1);
  else {
    if (d < 0)
      return luaL_error(L, "field " LUA_QS " missing in date table", key);
    res = d;
  }
  lua_pop(L, 1);
  return res;
}

static int os_time (lua_State *L){
	time_t t;

	if (lua_isnoneornil(L, 1)){
		t = time(NULL);
		t = mktime(gmtime(&t));
	}else{
		struct tm ts;
		luaL_checktype(L, 1, LUA_TTABLE);
		lua_settop(L, 1);
		ts.tm_sec = getfield(L, "sec", 0);
		ts.tm_min = getfield(L, "min", 0);
		ts.tm_hour = getfield(L, "hour", 12);
		ts.tm_mday = getfield(L, "day", -1);
		ts.tm_mon = getfield(L, "month", -1) - 1;
		ts.tm_year = getfield(L, "year", -1) - 1900;
		ts.tm_isdst = getboolfield(L, "isdst");
		t = mktime(&ts);
	}

	if(t == (time_t)(-1)){
		lua_pushnil(L);
	}else{
		lua_pushnumber(L, (lua_Number)t);
	}
	return 1;
}


static int os_difftime (lua_State *L){
	lua_pushnumber(L, difftime((time_t)(luaL_checknumber(L, 1)), (time_t)(luaL_optnumber(L, 2, 0))));
	return 1;
}

static const luaL_Reg syslib[] = {
  {"difftime", os_difftime},
  {"time", os_time},
  {NULL, NULL}
};

int luaopen_obos(lua_State *L){
	luaL_register(L, "os", syslib);
	return 1;
}
