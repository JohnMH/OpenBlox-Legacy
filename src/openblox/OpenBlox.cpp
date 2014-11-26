#include "OpenBlox.h"

int main(){
	static_init::execute();

	OpenBlox::BaseGame *game = new OpenBlox::BaseGame();

	lua_State *L = OpenBlox::BaseGame::getGlobalState();
	char* script = "for i, v in pairs(_G) do print(i, v); end warn('Finished printing environment');";
	int s = luaL_loadbuffer(L, script, strlen(script), "@game.Workspace.Script");
	if(s == 0){
		s = lua_pcall(L, 0, LUA_MULTRET, 0);
	}

	if(s != 0){
		game->handle_errors(L);
	}
	lua_close(L);

	return 0;
}
