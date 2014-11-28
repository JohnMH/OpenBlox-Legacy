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
	};

	STATIC_INIT(DataModel){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new DataModelClassMaker());

		lua_State *L = OpenBlox::BaseGame::getGlobalState();

		luaL_Reg methods[] = {
			{"__tostring", lua_Instance_toString},
			{NULL, NULL}
		};
	}

	char* DataModel::ClassName = "DataModel";

	DataModel::DataModel() : Instance(){

	}

	DataModel::~DataModel(){

	}

	Instance* DataModel::cloneImpl(){
		return NULL;
	}

	int DataModel::wrap_lua(lua_State *L){

	}

	 char* DataModel::toString(){
		 return ClassName;
	 }
}
