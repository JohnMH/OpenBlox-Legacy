#include "StarterGui.h"

#include "../ob_type/Vector2.h"

#include "ScreenGui.h"

#include "../openblox/OpenBloxRenderUtil.h"

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

		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
	}

	char* StarterGui::ClassName = "StarterGui";
	char* StarterGui::LuaClassName = "luaL_Instance_StarterGui";

	StarterGui::StarterGui() : BasePlayerGui(){
		Name = ClassName;
		ShowDevelopmentGui = true;
	}

	StarterGui::~StarterGui(){}

	void StarterGui::sizeChanged(int width, int height){
		ob_type::Vector2* sizeVec = new ob_type::Vector2(width, height);

		for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
			Instance* kid = children[i];
			if(kid != NULL){
				if(ScreenGui* sg = dynamic_cast<ScreenGui*>(kid)){
					sg->setAbsoluteSize(sizeVec);
				}
			}
		}

		delete sizeVec;
	}

	void StarterGui::Destroy(){}

	void StarterGui::render(){
		if(ShowDevelopmentGui){
			int width;
			int height;
			OpenBlox::getFramebufferSize(&width, &height);

			glMatrixMode(GL_PROJECTION);
			{
				glLoadIdentity();
				glOrtho(0, width, height, 0, 1.f, -1.f);
			}

			glMatrixMode(GL_MODELVIEW);

			for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
				Instance* kid = children[i];
				if(kid != NULL){
					if(ScreenGui* sg = dynamic_cast<ScreenGui*>(kid)){
						sg->render();
					}
				}
			}
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

	void StarterGui::addChild(Instance* kid){
		Instance::addChild(kid);

		if(ScreenGui* sg = dynamic_cast<ScreenGui*>(kid)){
			int width;
			int height;
			OpenBlox::getFramebufferSize(&width, &height);

			ob_type::Vector2* newVec2 = new ob_type::Vector2(width, height);

			sg->setAbsoluteSize(newVec2);

			delete newVec2;
		}
	}

	void StarterGui::removeChild(Instance* kid){
		Instance::removeChild(kid);

		if(kid != NULL){
			if(ScreenGui* sg = dynamic_cast<ScreenGui*>(kid)){
				ob_type::Vector2* newVec2 = new ob_type::Vector2(0, 0);

				sg->setAbsoluteSize(newVec2);

				delete newVec2;
			}
		}
	}
}
