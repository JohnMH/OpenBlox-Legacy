#ifndef OPENBLOX_FACTORY_H_
#define OPENBLOX_FACTORY_H_

#include "OpenBlox.h"

namespace ob_instance{
	class Instance;
}

namespace OpenBlox{
	struct ClassMaker{
		virtual ob_instance::Instance* getInstance() const = 0;
		virtual bool isA(const ob_instance::Instance* obj) = 0;
		virtual bool isInstantiatable() = 0;
		virtual bool isService(bool isDataModel) = 0;
		virtual ~ClassMaker(){}
	};

	class Factory{
		private:
			std::map<std::string, ClassMaker*> lokupTable;
			std::vector<std::string> registered;
		public:
			std::vector<std::string> getRegisteredMetatables();
			void addClass(std::string className, ClassMaker* const newClassMaker);
			ob_instance::Instance* create(std::string className);
			ob_instance::Instance* createService(std::string className, bool isDataModel);
			bool isA(const ob_instance::Instance* obj, std::string className);
			void releaseTable();
	};
}
#endif
