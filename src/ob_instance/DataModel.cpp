#include "DataModel.h"

#ifndef OPENBLOX_SERVER
#include "../openblox/OpenBloxRenderUtil.h"
#endif

namespace ob_instance{
	DEFINE_CLASS(DataModel, false, false);

	DataModel::DataModel() : ServiceProvider(){
		Name = ClassName;
		ParentLocked = true;

		starterGui = new StarterGui();
		starterGui->setParent(this);
		starterGui->parentLock();

		runService = new RunService();
		runService->setParent(this);
		runService->parentLock();

		title = "OpenBlox";
	}

	DataModel::~DataModel(){}

	Instance* DataModel::cloneImpl(){
		return NULL;
	}

	Instance* DataModel::GetService(std::string className){
		Instance* foundService = FindService(className);
		if(foundService != NULL){
			return foundService;
		}
		ob_instance::Instance* newGuy = OpenBlox::BaseGame::getInstanceFactory()->createService(className, true);
		if(newGuy){
			newGuy->setParent(this);
			newGuy->parentLock();
		}
		return newGuy;
	}

	void DataModel::render(){
		starterGui->render();
	}

	void DataModel::register_lua_property_setters(lua_State* L){
		Instance::register_lua_property_setters(L);
	}

	void DataModel::register_lua_property_getters(lua_State* L){
		Instance::register_lua_property_getters(L);
	}

	//TODO: Implement these as methods of DataModel. We want it to be possible to write an OpenBlox game entirely in C++ if desired.
	void DataModel::register_lua_methods(lua_State* L){
		luaL_Reg methods[]{
			{"SetTitle", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(DataModel* dm = dynamic_cast<DataModel*>(inst)){
					dm->title = std::string(luaL_checkstring(L, 2));
					#ifndef OPENBLOX_SERVER
						glfwSetWindowTitle(OpenBlox::getWindow(), dm->title.c_str());
					#endif
					return 0;
				}
				return luaL_error(L, COLONERR, "SetTitle");
			}},
			{"GetTitle", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(DataModel* dm = dynamic_cast<DataModel*>(inst)){
					lua_pushstring(L, dm->title.c_str());
					return 1;
				}
				return luaL_error(L, COLONERR, "GetTitle");
			}},
			{"GetFPS", [](lua_State* L)->int{
				lua_pushnumber(L, OpenBlox::calculatedFPS);
				return 1;
			}},
			{NULL,NULL}
		};
		luaL_register(L, NULL, methods);
		ServiceProvider::register_lua_methods(L);
	}
}
