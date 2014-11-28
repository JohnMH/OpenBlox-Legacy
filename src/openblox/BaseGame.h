#ifndef OPENBLOX_BASEGAME_H_
#define OPENBLOX_BASEGAME_H_

#include "OpenBlox.h"

namespace ob_instance{
	class DataModel;
}

namespace OpenBlox{
	class Factory;

	class BaseGame{
		public:
			BaseGame();
			virtual ~BaseGame();

			int print(lua_State *L);
			int warn(lua_State *L);
			void handle_errors(lua_State *L);

			ob_instance::DataModel* getDataModel();

			static lua_State *getGlobalState();
			static Factory *getInstanceFactory();
			static Factory *InstanceFactory;

			static BaseGame *getInstance();

		private:
			lua_State *newLuaState();
			ob_instance::DataModel *datamodel;

			static lua_State *GlobalLuaState;
	};
}
#endif
