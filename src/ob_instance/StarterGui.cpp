#include "StarterGui.h"

#include "../ob_type/Vector2.h"

#include "ScreenGui.h"

#include "GuiObject.h"

#ifndef OPENBLOX_SERVER
#include "../openblox/OpenBloxRenderUtil.h"
#endif

namespace ob_instance{
	DEFINE_CLASS(StarterGui, false, isDataModel);

	StarterGui::StarterGui() : BasePlayerGui(){
		Name = ClassName;
		ShowDevelopmentGui = true;
	}

	StarterGui::~StarterGui(){}

	Instance* StarterGui::cloneImpl(){
		return NULL;
	}

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
		#ifndef OPENBLOX_SERVER
		if(ShowDevelopmentGui){
			int width;
			int height;
			OpenBlox::getFramebufferSize(&width, &height);

			glMatrixMode(GL_PROJECTION);
			{
				glLoadIdentity();
				glOrtho(0, width, height, 0, -1.f, 1.f);
			}

			glMatrixMode(GL_MODELVIEW);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
				Instance* kid = children[i];
				if(kid != NULL){
					if(ScreenGui* sg = dynamic_cast<ScreenGui*>(kid)){
						sg->render();
					}
				}
			}
		}
		#endif
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

	void StarterGui::onClick(int x, int y, ob_enum::MouseButton button, bool down){
		std::vector<GuiObject*> kids = std::vector<GuiObject*>();

		for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
			Instance* kid = children[i];
			if(kid != NULL){
				if(ScreenGui* sg = dynamic_cast<ScreenGui*>(kid)){
					sg->giveUpKids(&kids);
				}
			}
		}

		std::sort(kids.begin(), kids.end(), [](GuiObject* go1, GuiObject* go2){
			return go1->ZIndex < go2->ZIndex;
		});

		GuiObject* clickedOn = NULL;

		for(std::vector<GuiObject*>::size_type i = 0; i != kids.size(); i++){
			GuiObject* go = kids[i];
			if(go != NULL){ //You can never be too careful :)
				if(go->containsPoint(x, y)){
					Instance* parent = go->getParent();
					while(parent != NULL){
						if(GuiObject* pgo = dynamic_cast<GuiObject*>(parent)){
							if(pgo->ClipsDescendants){
								if(!pgo->containsPoint(x, y)){
									continue;
								}
							}
							parent = pgo->getParent();
						}else{
							parent = NULL;
						}
					}
					clickedOn = go;
					break;
				}
			}
		}

		if(clickedOn != NULL){
			clickedOn->BackgroundColor3 = new ob_type::Color3(1, 0, 0);
		}
	}
}
