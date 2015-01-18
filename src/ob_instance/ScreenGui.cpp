#include "ScreenGui.h"

#include "GuiObject.h"

namespace ob_instance{
	DEFINE_CLASS(ScreenGui, true, false);

	ScreenGui::ScreenGui() : LayerCollector(){}

	ScreenGui::~ScreenGui(){}

	Instance* ScreenGui::cloneImpl(){
		ScreenGui* newGuy = new ScreenGui;
		newGuy->Name = Name;
		return newGuy;
	}

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
