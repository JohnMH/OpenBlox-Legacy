/*
Example Use:
struct InstanceCreator: public OpenBlox::ClassMaker{
	void* getInstance() const{
		return new Instance;
	}
};

STATIC_INIT(Instance){
	OpenBlox::BaseGame::getInstanceFactory()->addClass("instance", new InstanceCreator());
}
 */

#ifndef OPENBLOX_FACTORY_H_
#define OPENBLOX_FACTORY_H_

#include "OpenBlox.h"

namespace OpenBlox{
	struct ClassMaker{
		virtual void* getInstance() const = 0;
		virtual ~ClassMaker(){}
	};

	class Factory{
		private:
			std::map<const char*, ClassMaker*> lokupTable;
		public:
			void addClass(const char* className, ClassMaker* const newClassMaker);
			void* create(const char* className);
			void releaseTable();
	};
}
#endif
