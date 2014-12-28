#include "BaseGame.h"

#include "../ob_instance/DataModel.h"
#include "../ob_instance/HttpService.h"

#include "../ob_type/Vector3.h"
#include "../ob_type/Vector2.h"
#include "../ob_type/Color3.h"
#include "../ob_type/UDim.h"
#include "../ob_type/UDim2.h"

#include <locale>

#include <cwchar>
#include <ctime>

#include "oboslib.h"

#include <curl/curl.h>

namespace OpenBlox{
	static BaseGame* INSTANCE;
	static long APP_START = currentTimeMillis();

	static std::map<std::string, std::string> libMap;

	lua_State* BaseGame::GlobalLuaState = NULL;
	Factory* BaseGame::InstanceFactory = NULL;

	BaseGame::BaseGame(){
		INSTANCE = this;
		datamodel = new ob_instance::DataModel();

		libMap = std::map<std::string, std::string>();
		libMap["RbxUtility"] = "https://raw.githubusercontent.com/RobloxLabs/internal-code/master/library-scripts/RbxUtility-60595411.lua";
		libMap["RbxGear"] = "https://raw.githubusercontent.com/RobloxLabs/internal-code/master/library-scripts/RbxGear-45374389.lua";
		libMap["RbxStamper"] = "https://raw.githubusercontent.com/RobloxLabs/internal-code/master/library-scripts/RbxStamper-73157242.lua";
		libMap["RbxGui"] = "https://raw.githubusercontent.com/RobloxLabs/internal-code/master/library-scripts/RbxGui-45284430.lua";

		GlobalLuaState = lua_open();
	}

	BaseGame::~BaseGame(){
		INSTANCE = NULL;
	}

	ob_instance::DataModel* BaseGame::getDataModel(){
		return datamodel;
	}

	//TODO: Implement LogService print, warn, error
	void BaseGame::print(std::string output){
		LOGI(output.c_str());
	}

	void BaseGame::warn(std::string output){
		LOGW(output.c_str());
	}

	void BaseGame::print_error(std::string output){
		LOGE(output.c_str());
	}

	void BaseGame::handle_lua_errors(lua_State* L){
		const char* output = lua_tostring(L, -1);
		if(INSTANCE != NULL){
			INSTANCE->print_error(output);
		}
		lua_pop(L, 1);
	}

	lua_State* BaseGame::newLuaState(){
		lua_State* L = lua_newthread(GlobalLuaState);

		luaopen_base(L);

		luaopen_table(L);
		luaopen_string(L);
		luaopen_math(L);

		luaopen_obos(L);

		lua_pushnil(L);
		lua_setglobal(L, "dofile");
		lua_pushnil(L);
		lua_setglobal(L, "load");
		lua_pushnil(L);
		lua_setglobal(L, "loadfile");

		lua_register(L, "delay", lua_delay);
		lua_register(L, "Delay", lua_delay);
		lua_register(L, "elapsedTime", lua_elapsedTime);
		lua_register(L, "ElapsedTime", lua_elapsedTime);
		lua_register(L, "spawn", lua_spawn);
		lua_register(L, "Spawn", lua_spawn);
		lua_register(L, "print", lua_print);
		lua_register(L, "warn", lua_warn);
		lua_register(L, "tick", lua_tick);
		lua_register(L, "wait", lua_wait);
		lua_register(L, "Wait", lua_wait);
		lua_register(L, "LoadLibrary", lua_loadlibrary);

		luaL_Reg instancelib[]{
			{"new", lua_newInstance},
			{NULL, NULL}
		};
		luaL_register(L, "Instance", instancelib);

		luaL_Reg vector3lib[]{
			{"new", lua_newVector3},
			{NULL, NULL}
		};
		luaL_register(L, "Vector3", vector3lib);

		luaL_Reg vector2lib[]{
			{"new", lua_newVector2},
			{NULL, NULL}
		};
		luaL_register(L, "Vector2", vector2lib);

		luaL_Reg color3lib[]{
			{"new", lua_newColor3},
			{NULL, NULL}
		};
		luaL_register(L, "Color3", color3lib);

		luaL_Reg udimlib[]{
			{"new", lua_newUDim},
			{NULL, NULL}
		};
		luaL_register(L, "UDim", udimlib);

		luaL_Reg udim2lib[]{
			{"new", lua_newUDim2},
			{NULL, NULL}
		};
		luaL_register(L, "UDim2", udim2lib);

		ob_instance::DataModel* dm = INSTANCE->getDataModel();
		int gm = dm->wrap_lua(L);
		lua_pushvalue(L, -gm);
		lua_setglobal(L, "game");

		lua_pushvalue(L, -gm);
		lua_setglobal(L, "Game");

		lua_pop(L, gm);

		return L;
	}

	lua_State* BaseGame::getGlobalState(){
		return GlobalLuaState;
	}

	Factory* BaseGame::getInstanceFactory(){
		return InstanceFactory;
	}

	long BaseGame::appStarted(){
		return APP_START;
	}

	BaseGame* BaseGame::getInstance(){
		return INSTANCE;
	}

	double BaseGame::elapsedTime(){
		return (currentTimeMillis() - APP_START) / 1000.0;
	}

	int BaseGame::lua_wait(lua_State* L){
		double waitTime = 1/60;
		if(!lua_isnoneornil(L, 1)){
			waitTime = luaL_checknumber(L, 1);
		}
		return ThreadScheduler::Wait(L, (waitTime * 1000));
	}

	int BaseGame::lua_delay(lua_State* L){
		if(lua_isnumber(L, 1)){
			double delayTime = luaL_checknumber(L, 1);

			luaL_checktype(L, 2, LUA_TFUNCTION);

			return ThreadScheduler::Delay(L, 2, delayTime * 1000);
		}else{
			luaL_checktype(L, 1, LUA_TFUNCTION);

			double delayTime = luaL_checknumber(L, 2);

			return ThreadScheduler::Delay(L, 1, delayTime * 1000);
		}
	}

	int BaseGame::lua_spawn(lua_State* L){
		luaL_checktype(L, 1, LUA_TFUNCTION);
		return ThreadScheduler::Spawn(L, 1);
	}

	int BaseGame::lua_loadlibrary(lua_State* L){
		std::string libName = std::string(luaL_checkstring(L, 1));
		std::map<std::string, std::string>::iterator it = libMap.find(libName);
		if(it != libMap.end()){
			struct ob_instance::HttpService::response_body body;
			body.size = 0;
			body.data = new char[4096];
			if(body.data == NULL){
				LOGE("[LoadLibrary] Failed to allocate memory.");
				return luaL_error(L, "Error loading library %s", libName.c_str());
			}

			body.data[0] = '\0';

			CURL* curl;
			CURLcode res;

			curl = curl_easy_init();
			if(curl){
				curl_easy_setopt(curl, CURLOPT_URL, it->second.c_str());

				curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);

				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ob_instance::HttpService::write_data);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);

				res = curl_easy_perform(curl);
				if(res != CURLE_OK){
					LOGE("[LoadLibrary] cURL Error: %s", curl_easy_strerror(res));
					return luaL_error(L, "Error loading library %s", libName.c_str());
				}

				curl_easy_cleanup(curl);

				int s = luaL_loadbuffer(L, body.data, strlen(body.data), libName.c_str());
				if(s == 0){
					s = lua_pcall(L, 0, 1, 0);
					if(s == 0){
						return 1;
					}else{

					}
				}else{
					return luaL_error(L, "Error loading library %s", libName.c_str());
				}
			}
		}
		return luaL_error(L, "Unknown library %s", libName.c_str());
	}

	int BaseGame::lua_tick(lua_State* L){
		time_t t = time(NULL);
		t = mktime(localtime(&t));

		lua_pushnumber(L, t);
		return 1;
	}

	int BaseGame::lua_print(lua_State* L){
		std::string output = "";

		int n = lua_gettop(L);
		int i;

		lua_getglobal(L, "tostring");
		for(i=1; i <= n; i++){
			const char* s;
			lua_pushvalue(L, -1);
			lua_pushvalue(L, i);
			lua_call(L, 1, 1);
			s = lua_tostring(L, -1);
			lua_pop(L, 1);
			if(s == NULL){
				return luaL_error(L, LUA_QL("tostring") " must return a string to " LUA_QL("print"));
			}
			if(i > 1){
				output = output + "\t";
			}
			output = output + s;
		}

		if(INSTANCE != NULL){
			INSTANCE->print(output);
		}

		return 0;
	}

	int BaseGame::lua_elapsedTime(lua_State* L){
		lua_pushnumber(L, elapsedTime());
		return 1;
	}

	int BaseGame::lua_warn(lua_State* L){
		std::string output = "";

		int n = lua_gettop(L);
		int i;

		lua_Debug ar;
		if(lua_getstack(L, 1, &ar)){
			lua_getinfo(L, "Sl", &ar);
			if(ar.currentline > 0){
				output = output + ar.short_src + ":" + ((std::ostringstream&)(std::ostringstream() << std::dec << ar.currentline)).str() + ": ";
			}
		}

		lua_getglobal(L, "tostring");
		for(i=1; i <= n; i++){
			const char* s;
			lua_pushvalue(L, -1);
			lua_pushvalue(L, i);
			lua_call(L, 1, 1);
			s = lua_tostring(L, -1);
			if(s == NULL){
				return luaL_error(L, LUA_QL("tostring") " must return a string to " LUA_QL("print"));
			}
			if(i > 1){
				output = output + "\t";
			}
			output = output + s;
			lua_pop(L, 1);
		}

		if(INSTANCE != NULL){
			INSTANCE->warn(output);
		}

		return 0;
	}

	int BaseGame::lua_newInstance(lua_State* L){
		std::string className = std::string(luaL_checkstring(L, 1));
		ob_instance::Instance* par = ob_instance::Instance::checkInstance(L, 2);
		if(BaseGame::InstanceFactory != NULL){
			ob_instance::Instance* newGuy = BaseGame::InstanceFactory->create(className);
			if(newGuy != NULL){
				if(par != NULL){
					try{
						newGuy->setParent(par);
					}catch(std::runtime_error& ex){
						return luaL_error(L, ex.what());
					}
				}
				return newGuy->wrap_lua(L);
			}
		}
		lua_pushnil(L);
		return 1;
	}

	int BaseGame::lua_newVector3(lua_State* L){
		double x = luaL_checknumber(L, 1);
		double y = luaL_checknumber(L, 2);
		double z = luaL_checknumber(L, 3);

		ob_type::Vector3* newGuy = new ob_type::Vector3(x, y, z);
		return newGuy->wrap_lua(L);
	}

	int BaseGame::lua_newVector2(lua_State* L){
		double x = luaL_checknumber(L, 1);
		double y = luaL_checknumber(L, 2);

		ob_type::Vector2* newGuy = new ob_type::Vector2(x, y);
		return newGuy->wrap_lua(L);
	}

	int BaseGame::lua_newColor3(lua_State* L){
		double r = 0;
		double g = 0;
		double b = 0;

		if(!lua_isnone(L, 1) && !lua_isnone(L, 2) && !lua_isnone(L, 3)){
			r = luaL_checknumber(L, 1);
			g = luaL_checknumber(L, 2);
			b = luaL_checknumber(L, 3);
		}

		ob_type::Color3* newGuy = new ob_type::Color3(r, g, b);
		return newGuy->wrap_lua(L);
	}

	int BaseGame::lua_newUDim(lua_State* L){
		double scale = luaL_checknumber(L, 1);
		int offset = luaL_checkint(L, 2);

		ob_type::UDim* newGuy = new ob_type::UDim(scale, offset);
		return newGuy->wrap_lua(L);
	}

	int BaseGame::lua_newUDim2(lua_State* L){
		double xscale = luaL_checknumber(L, 1);
		int xoffset = luaL_checkint(L, 2);
		double yscale = luaL_checknumber(L, 3);
		int yoffset = luaL_checkint(L, 4);

		ob_type::UDim2* newGuy = new ob_type::UDim2(xscale, xoffset, yscale, yoffset);
		return newGuy->wrap_lua(L);
	}
}
