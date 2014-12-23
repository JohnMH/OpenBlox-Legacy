#include "StarterGui.h"

namespace ob_instance{
	struct StarterGuiClassMaker: public OpenBlox::ClassMaker{
		ob_instance::Instance* getInstance() const{
			return new StarterGui;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const StarterGui*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return false;
		}

		bool isService(bool isDataModel){
			return isDataModel;
		}
	};

	STATIC_INIT(StarterGui){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new StarterGuiClassMaker());

		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, Instance::register_lua_events);
	}

	char* StarterGui::ClassName = "StarterGui";
	char* StarterGui::LuaClassName = "luaL_Instance_StarterGui";

	StarterGui::StarterGui() : BasePlayerGui(){
		Name = ClassName;
		ShowDevelopmentGui = true;
	}

	StarterGui::~StarterGui(){}

	void StarterGui::render(){
		if(ShowDevelopmentGui){
			glLoadIdentity();
			glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

			glBegin(GL_TRIANGLES);
			{
				glColor3f(1.f, 0.f, 0.f);
				glVertex3f(-0.6f, -0.4f, 0.f);
				glColor3f(0.f, 1.f, 0.f);
				glVertex3f(0.6f, -0.4f, 0.f);
				glColor3f(0.f, 0.f, 1.f);
				glVertex3f(0.f, 0.6f, 0.f);
			}
			glEnd();
		}
	}

	int StarterGui::wrap_lua(lua_State* L){
		StarterGui** udata = (StarterGui**)lua_newuserdata(L, sizeof(*this));
		*udata = this;

		luaL_getmetatable(L, LuaClassName);
		lua_setmetatable(L, -2);

		return 1;
	}

	Instance* StarterGui::cloneImpl(){
		return NULL;
	}

	char* StarterGui::getClassName(){
		return ClassName;
	}
}
