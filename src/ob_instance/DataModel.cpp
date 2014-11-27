#include "DataModel.h"

namespace ob_instance{
	struct DataModelClassMaker: public OpenBlox::ClassMaker{
		void* getInstance() const{
			return NULL;
		}
		bool isA(ob_instance::Instance* obj){
			return (dynamic_cast<DataModel*>(obj)) != 0;
		}
		bool isInstantiatable(){
			return false;
		}
	};

	STATIC_INIT(DataModel){
		OpenBlox::BaseGame::getInstanceFactory()->addClass("DataModel", new DataModelClassMaker());
	}

	DataModel::DataModel(){

	}

	DataModel::~DataModel(){

	}

	void DataModel::wrap_lua_impl(lua_State *L){

	}
}
