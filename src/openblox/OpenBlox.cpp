#include "OpenBlox.h"

#include "../ob_instance/DataModel.h"

int main(){
	OpenBlox::BaseGame::InstanceFactory = new OpenBlox::Factory();

	OpenBlox::BaseGame *game = new OpenBlox::BaseGame();

	static_init::execute();

	lua_State *L = OpenBlox::BaseGame::getGlobalState();

	ob_instance::DataModel *dm = game->getDataModel();


	char* script = "for i, v in pairs(_G) do print(i, v); end warn('Finished printing environment');";
	int s = luaL_loadbuffer(L, script, strlen(script), "@game.Workspace.Script");
	if(s == 0){
		s = lua_pcall(L, 0, LUA_MULTRET, 0);
	}

	if(s != 0){
		game->handle_errors(L);
	}
	lua_close(L);

	OpenBlox::BaseGame::getInstanceFactory()->releaseTable();
	return 0;
}
