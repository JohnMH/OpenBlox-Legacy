#ifndef OB_INSTANCE_GUIBASE2D_H_
#define OB_INSTANCE_GUIBASE2D_H_

#include "GuiBase.h"

#include "../ob_type/Vector2.h"

namespace ob_instance{
	class GuiBase2d: public GuiBase{
		public:
			GuiBase2d();
			virtual ~GuiBase2d();

			ob_type::Vector2* AbsolutePosition;
			ob_type::Vector2* AbsoluteSize;

			virtual void sizeChanged();

			void setAbsolutePosition(ob_type::Vector2* newVec2);
			ob_type::Vector2* getAbsolutePosition();

			void setAbsoluteSize(ob_type::Vector2* newVec2);
			ob_type::Vector2* getAbsoluteSize();

			virtual std::string getClassName();

			static int lua_getAbsolutePosition(lua_State* L);
			static int lua_getAbsoluteSize(lua_State* L);

			DECLARE_STATIC_INIT(GuiBase2d);
		protected:
			static std::string ClassName;
			static std::string LuaClassName;

			static void register_lua_property_setters(lua_State* L);
			static void register_lua_property_getters(lua_State* L);
	};
};
#endif
