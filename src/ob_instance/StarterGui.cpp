#include "StarterGui.h"

#include "../ob_type/Vector2.h"

#include "ScreenGui.h"

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

	bool StarterGui::isMouseCaptured(int x, int y){
		for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
			Instance* kid = children[i];
			if(kid != NULL){
				if(ScreenGui* sg = dynamic_cast<ScreenGui*>(kid)){
					if(sg->isMouseCaptured(x, y)){
						return true;
					}
				}
			}
		}

		return false;
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
}
