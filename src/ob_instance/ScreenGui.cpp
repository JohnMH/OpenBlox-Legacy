#include "ScreenGui.h"

#include "GuiObject.h"

namespace ob_instance{
	struct ScreenGuiClassMaker: public OpenBlox::ClassMaker{
		ob_instance::Instance* getInstance() const{
			return new ScreenGui;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const ScreenGui*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return true;
		}

		bool isService(bool isDataModel){
			return false;
		}
	};

	STATIC_INIT(ScreenGui){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new ScreenGuiClassMaker());

		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
	}
	char* ScreenGui::ClassName = "ScreenGui";
	char* ScreenGui::LuaClassName = "luaL_Instance_ScreenGui";

	ScreenGui::ScreenGui() : LayerCollector(){}

	ScreenGui::~ScreenGui(){}

	void ScreenGui::render(){
		for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
			Instance* kid = children[i];
			if(kid != NULL){
				if(GuiObject* go = dynamic_cast<GuiObject*>(kid)){
					go->render();
				}
			}
		}
	}

	bool ScreenGui::isMouseCaptured(int x, int y){
		for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
			Instance* kid = children[i];
			if(kid != NULL){
				if(GuiObject* go = dynamic_cast<GuiObject*>(kid)){
					if(go->isMouseCaptured(x, y)){
						return true;
					}
				}
			}
		}

		return false;
	}

	int ScreenGui::wrap_lua(lua_State* L){
		ScreenGui** udata = (ScreenGui**)lua_newuserdata(L, sizeof(*this));
		*udata = this;

		luaL_getmetatable(L, LuaClassName);
		lua_setmetatable(L, -2);

		return 1;
	}

	Instance* ScreenGui::cloneImpl(){
		ScreenGui* newGuy = new ScreenGui;
		newGuy->Name = Name;
		return newGuy;
	}

	char* ScreenGui::getClassName(){
		return ClassName;
	}

	void ScreenGui::addChild(Instance* kid){
		Instance::addChild(kid);

		if(GuiObject* go = dynamic_cast<GuiObject*>(kid)){
			go->sizeChanged();
		}
	}

	void ScreenGui::removeChild(Instance* kid){
		Instance::removeChild(kid);

		if(kid != NULL){
			if(GuiObject* sg = dynamic_cast<GuiObject*>(kid)){
				ob_type::Vector2* newVec2 = new ob_type::Vector2(0, 0);

				sg->setAbsoluteSize(newVec2);

				delete newVec2;
			}
		}
	}
}
