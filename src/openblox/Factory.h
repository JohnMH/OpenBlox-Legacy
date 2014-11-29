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
		virtual bool isService() = 0;
		virtual ~ClassMaker(){}
	};

	class Factory{
		private:
			std::map<std::string, ClassMaker*> lokupTable;
			std::vector<const char*> registered;
		public:
			std::vector<const char*> getRegisteredMetatables();
			void addClass(const char* className, ClassMaker* const newClassMaker);
			ob_instance::Instance* create(const char* className);
			bool isA(const ob_instance::Instance* obj, const char* className);
			void releaseTable();
	};
}
#endif
