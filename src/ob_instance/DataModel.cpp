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
		OpenBlox::BaseGame::getInstanceFactory()->addClass("DataModel", new DataModelClassMaker());

		//std::cout << OpenBlox::BaseGame::getGlobalState() << std::endl;
	}

	DataModel::DataModel(){

	}

	DataModel::~DataModel(){

	}

	void DataModel::wrap_lua(lua_State *L){

	}
}
