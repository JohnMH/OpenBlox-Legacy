#include <iostream>
#include <cstring>

extern "C"{
	#include "../lua.h"
	#include "../lualib.h"
	#include "../lauxlib.h"
}

//TODO: Implement LogService print, warn, error
int openblox_print(lua_State *L){
	int n = lua_gettop(L);
	int i;
	lua_getglobal(L, "tostring");
	for(i=1; i <= n; i++){
		const char *s;
		lua_pushvalue(L, -1);
		lua_pushvalue(L, i);
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);
		if(s == NULL){
			return luaL_error(L, LUA_QL("tostring") " must return a string to " LUA_QL("print"));
		}
		if(i>1){
			fputs("\t", stdout);
		}
		fputs(s, stdout);
		lua_pop(L, 1);
	}
	fputs("\n", stdout);
	return 0;
}

int openblox_warn(lua_State *L){
	int n = lua_gettop(L);
	int i;
	lua_getglobal(L, "tostring");
	for(i=1; i <= n; i++){
		const char *s;
		lua_pushvalue(L, -1);
		lua_pushvalue(L, i);
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);
		if(s == NULL){
			return luaL_error(L, LUA_QL("tostring") " must return a string to " LUA_QL("print"));
		}
		if(i>1){
			fputs("\t", stdout);
		}
		fputs(s, stdout);
		lua_pop(L, 1);
	}
	fputs("\n", stdout);
	return 0;
}

void handle_errors(lua_State *L, int status){
	if(status != 0){
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}
}

//Lua reader
typedef struct LoadS{
	const char *s;
	size_t size;
} LoadS;

static const char *getS (lua_State *L, void *ud, size_t *size){
	LoadS *ls = (LoadS *)ud;
	(void)L;
	if(ls->size == 0){
		return NULL;
	}
	*size = ls->size;
	ls->size = 0;
	return ls->s;
}

int main(){
	lua_State *L = lua_open();
	luaopen_base(L);
	luaopen_table(L);
	luaopen_string(L);
	luaopen_math(L);

	lua_pushnil(L);
	lua_setglobal(L, "dofile");
	lua_pushnil(L);
	lua_setglobal(L, "load");
	lua_pushnil(L);
	lua_setglobal(L, "loadfile");

	lua_register(L, "print", openblox_print);
	lua_register(L, "warn", openblox_warn);

	//int s = luaL_loadstring(L, "print('Hello world, from ' .. _VERSION .. '!')");
	char* script = "for i, v in pairs(_G) do print(i, v); end warn('Finished printing environment');";
	int s = luaL_loadbuffer(L, script, strlen(script), "@game.Workspace.Script");
	if(s == 0){
		s = lua_pcall(L, 0, LUA_MULTRET, 0);
	}

	handle_errors(L, s);
	lua_close(L);

	return 0;
}
