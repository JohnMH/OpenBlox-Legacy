#ifndef OB_INSTANCE_GUIOBJECT_H_
#define OB_INSTANCE_GUIOBJECT_H_

#include "GuiBase2d.h"

#include "../ob_type/Color3.h"
#include "../ob_type/UDim2.h"

namespace ob_instance{
	class GuiObject: public GuiBase2d{
		public:
			GuiObject();
			virtual ~GuiObject();

			virtual void sizeChanged();

			bool isMouseCaptured(int x, int y);

			ob_type::Vector2* calculateRenderPosision(ob_type::Vector2* seed);
			ob_type::Vector2* calculateRenderSize(ob_type::Vector2* seed);

			DECLARE_STATIC_INIT(GuiObject);
		protected:
			static std::string ClassName;
			static std::string LuaClassName;

			ob_type::LuaEvent* DragBegin; //UDim2 initialPosition
			ob_type::LuaEvent* DragStopped; //int x, int y

			ob_type::LuaEvent* InputBegan; //InputObject input
			ob_type::LuaEvent* InputChanged; //InputObject input
			ob_type::LuaEvent* InputEnded; //InputObject input

			ob_type::LuaEvent* MouseEnter; //int x, int y
			ob_type::LuaEvent* MouseLeave; //int x, int y
			ob_type::LuaEvent* MouseMoved; //int x, int y
			ob_type::LuaEvent* MouseWheelBackward; //int x, int y
			ob_type::LuaEvent* MouseWheelForward; //int x, int y

			ob_type::LuaEvent* TouchLongPress; //Array touchPositions, UserInputState state
			ob_type::LuaEvent* TouchPan; //Array touchPositions, Vector2 totalTranslation, Vector2 velocity, UserInputState state
			ob_type::LuaEvent* TouchPinch; //Array touchPositions, double scale, double velocity, UserInputState state
			ob_type::LuaEvent* TouchRotate; //Array touchPositions, double rotation, double velocity, UserInputState state
			ob_type::LuaEvent* TouchSwipe; //Enum SwipeDirection swipeDirection, int numberOfTouches
			ob_type::LuaEvent* TouchTap; //Array touchPositions

			bool Active;
			ob_type::Color3* BackgroundColor3;
			double BackgroundTransparency;
			ob_type::Color3* BorderColor3;
			int BorderSizePixel;
			bool ClipsDescendants;
			bool Draggable;
			ob_type::UDim2* Position;
			double Rotation;
			ob_type::UDim2* Size;
			//Enum SizeConstraint
			bool Visible;
			int ZIndex;

			static void register_lua_methods(lua_State* L);
			static void register_lua_property_setters(lua_State* L);
			static void register_lua_property_getters(lua_State* L);
			static void register_lua_events(lua_State* L);
	};
};
#endif
