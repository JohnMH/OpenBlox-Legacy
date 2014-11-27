#ifndef OPENBLOX_FACTORY_H_
#define OPENBLOX_FACTORY_H_

#include "OpenBlox.h"

namespace ob_instance{
	class Instance;
}

namespace OpenBlox{
	struct ClassMaker{
		virtual void* getInstance() const = 0;
		virtual bool isA(const ob_instance::Instance* obj) = 0;
		virtual bool isInstantiatable() = 0;
		virtual ~ClassMaker(){}
	};

	class Factory{
		private:
			std::map<std::string, ClassMaker*> lokupTable;
		public:
			void addClass(const char* className, ClassMaker* const newClassMaker);
			void* create(const char* className);
			bool isA(const ob_instance::Instance* obj, const char* className);
			void releaseTable();
	};
}
#endif
