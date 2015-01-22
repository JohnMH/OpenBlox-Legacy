#include "DataModel.h"

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

	const char* DataModel::getTitle(){
		return gameTitle;
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

	void DataModel::register_lua_methods(lua_State* L){
		luaL_Reg methods[]{
				{"SetTitle", [](lua_State* L)->int{
					Instance* inst = checkInstance(L, 1);
					if(DataModel* dm = dynamic_cast<DataModel*>(inst)){
						if (lua_isstring(L, 2)) {
							dm->gameTitle = luaL_checkstring(L, 2);
						}
						return 0;
					}
					return luaL_error(L, COLONERR, "SetTitle");
				}},
				{NULL,NULL}
		};
		luaL_register(L, NULL, methods);
		ServiceProvider::register_lua_methods(L);
	}
}
