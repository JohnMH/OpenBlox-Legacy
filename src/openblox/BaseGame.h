#ifndef OPENBLOX_BASEGAME_H_
#define OPENBLOX_BASEGAME_H_

namespace OpenBlox{
	class BaseGame{
		public:
			BaseGame();
			virtual ~BaseGame();

			int print(lua_State *L);
			int warn(lua_State *L);
			void handle_errors(lua_State *L);

			lua_State* newLuaState();
	};
}

#endif
