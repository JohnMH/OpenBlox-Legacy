#include "Frame.h"

namespace ob_instance{
	struct FrameClassMaker: public OpenBlox::ClassMaker{
		ob_instance::Instance* getInstance() const{
			return new Frame;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const Frame*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return true;
		}

		bool isService(bool isDataModel){
			return false;
		}
	};

	STATIC_INIT(Frame){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new FrameClassMaker());

		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
	}

	char* Frame::ClassName = "Frame";
	char* Frame::LuaClassName = "luaL_Instance_Frame";

	Frame::Frame() : GuiObject(){
		Name = ClassName;
	}

	Frame::~Frame(){}

	void Frame::render(){
		if(Visible){
			glColor3d(BackgroundColor3->r, BackgroundColor3->g, BackgroundColor3->b);

			const GLdouble dubindex = ZIndex;

			const GLdouble vertices[] = {
				AbsolutePosition->x,  AbsolutePosition->y + AbsoluteSize->y, dubindex, //Bottom Left
				AbsolutePosition->x,  AbsolutePosition->y, dubindex, //Top Left
				AbsolutePosition->x + AbsoluteSize->x,  AbsolutePosition->y, dubindex, //Top Right
				AbsolutePosition->x + AbsoluteSize->x,  AbsolutePosition->y + AbsoluteSize->y, dubindex //Bottom Right
			};
			const GLubyte indices[] = {
				0, 1, 2,//Bottom Left, Top Left, Top Right
				0, 2, 3//Bottom Left, Top Right, Bottom Right
			};

			glEnableClientState(GL_VERTEX_ARRAY);

			glVertexPointer(3, GL_DOUBLE, 0, vertices);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

			glDisableClientState(GL_VERTEX_ARRAY);
		}
	}

	int Frame::wrap_lua(lua_State* L){
		Frame** udata = (Frame**)lua_newuserdata(L, sizeof(*this));
		*udata = this;

		luaL_getmetatable(L, LuaClassName);
		lua_setmetatable(L, -2);

		return 1;
	}

	Instance* Frame::cloneImpl(){
		Frame* newGuy = new Frame;
		newGuy->Name = Name;

		newGuy->Active = Active;
		newGuy->BackgroundColor3 = BackgroundColor3->clone();
		newGuy->BackgroundTransparency = BackgroundTransparency;
		newGuy->BorderColor3 = BorderColor3;
		newGuy->BorderSizePixel = BorderSizePixel;
		newGuy->ClipsDescendants = ClipsDescendants;
		newGuy->Draggable = Draggable;
		newGuy->Position = Position->clone();
		newGuy->Rotation = Rotation;
		newGuy->Size = Size->clone();
		newGuy->Visible = Visible;
		newGuy->ZIndex = ZIndex;

		return newGuy;
	}

	char* Frame::getClassName(){
		return ClassName;
	}
}
