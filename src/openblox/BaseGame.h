#ifndef OPENBLOX_BASEGAME_H_
#define OPENBLOX_BASEGAME_H_

#include "OpenBlox.h"

#ifndef OPENBLOX_JNI
#define notDefined
#endif

namespace ob_instance{
	class DataModel;
}

namespace OpenBlox{
	class Factory;
	class ThreadScheduler;

	class BaseGame{
		public:
			BaseGame();
			virtual ~BaseGame();

			void print(const char* output);
			void warn(const char* output);
			void print_error(const char* output);
			void handle_lua_errors(lua_State* L);

			ob_instance::DataModel* getDataModel();
			ThreadScheduler* getThreadScheduler();

			static lua_State* getGlobalState();
			static Factory* getInstanceFactory();
			static Factory* InstanceFactory;

			static lua_State* newLuaState();

			static BaseGame* getInstance();

		private:
			ob_instance::DataModel* datamodel;
			ThreadScheduler* GlobalThreadScheduler;

			static lua_State* GlobalLuaState;
	};
}
#endif
