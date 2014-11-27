#ifndef OPENBLOX_FACTORY_H_
#define OPENBLOX_FACTORY_H_

#include "OpenBlox.h"

#include "../ob_instance/Instance.h"

#ifndef OB_INSTANCE_INSTANCE_H_
namespace ob_instance{
	class Instance;
}
#endif

namespace OpenBlox{
	struct ClassMaker{
		virtual void* getInstance() const = 0;
		virtual bool isA(ob_instance::Instance* obj) = 0;
		virtual bool isInstantiatable() = 0;
		virtual ~ClassMaker(){}
	};

	class Factory{
		private:
			std::map<const char*, ClassMaker*> lokupTable;
		public:
			void addClass(const char* className, ClassMaker* const newClassMaker);
			void* create(const char* className);
			bool isA(ob_instance::Instance* obj, const char* className);
			void releaseTable();
	};
}
#endif
