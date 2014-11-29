#include "DataModel.h"

namespace ob_instance{
	struct DataModelClassMaker: public OpenBlox::ClassMaker{
		void* getInstance() const{
			return NULL;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const DataModel*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return false;
		}

		bool isService(){
			return false;
		}
	};

	STATIC_INIT(DataModel){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new DataModelClassMaker());

		lua_State* L = OpenBlox::BaseGame::getGlobalState();

		luaL_newmetatable(L, LuaClassName);

		Instance::register_lua_metamethods(L);

		lua_pushstring(L, "__index");
		lua_newtable(L);

		lua_pushstring(L, "ClassName");
		lua_pushstring(L, ClassName);
		lua_rawset(L, -3);

		register_lua_methods(L);

		lua_rawset(L, -3);
		lua_pop(L, 1);
	}

	char* DataModel::ClassName = "DataModel";
	char* DataModel::LuaClassName = "luaL_Instance_DataModel";

	DataModel::DataModel() : ServiceProvider(){
		Name = ClassName;
	}

	DataModel::~DataModel(){

	}

	Instance* DataModel::cloneImpl(){
		return NULL;
	}

	int DataModel::wrap_lua(lua_State* L){
		DataModel **udata = (DataModel**)lua_newuserdata(L, sizeof(DataModel*));
		*udata = this;

		luaL_getmetatable(L, LuaClassName);
		lua_setmetatable(L, -2);

		return 1;
	}

	 char* DataModel::toString(){
		 return ClassName;
	 }

	 void DataModel::register_lua_methods(lua_State* L){
		 ServiceProvider::register_lua_methods(L);
	 }
}
