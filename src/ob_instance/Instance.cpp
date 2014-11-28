#include "Instance.h"

namespace ob_instance{
	struct InstanceClassMaker: public OpenBlox::ClassMaker{
		void* getInstance() const{
			return NULL;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const Instance*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return false;
		}
	};

	STATIC_INIT(Instance){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new InstanceClassMaker());
	}

	char* Instance::ClassName = "Instance";

	Instance::Instance(){
		Archivable = true;
		Name = ClassName;
		Parent = NULL;

		children = std::vector<Instance*>();
	}

	Instance::~Instance(){

	}

	void Instance::ClearAllChildren(){
		for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
			Instance* kid = children[i];
			if(kid != NULL){
				kid->Remove();
			}
		}
	}

	Instance* Instance::Clone(){
		if(Archivable){
			Instance* newGuy = cloneImpl();
			if(newGuy == NULL){
				return NULL;
			}
			for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
				Instance* kid = children[i];
				if(kid != NULL){
					Instance* kidClone = kid->Clone();
					if(kidClone != NULL){
						kidClone->setParent(newGuy);
					}
				}
			}
		}
		return NULL;
	}

	void Instance::Destroy(){
		setParent(NULL);
		//Lock parent property
		//Disconnect all events
		for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
			Instance* kid = children[i];
			if(kid != NULL){
				kid->Destroy();
			}
		}
	}

	void Instance::Remove(){
		setParent(NULL);
		for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
			Instance* kid = children[i];
			if(kid != NULL){
				kid->Remove();
			}
		}
	}

	Instance* Instance::FindFirstChild(char* name, bool recursive){
		if(name == NULL){
			return NULL;
		}
		for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
			Instance* kid = children[i];
			if(kid != NULL){
				if(strcmp(name, kid->Name) == 0){
					return kid;
				}
			}
		}
		if(recursive){
			for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
				Instance* kid = children[i];
				if(kid != NULL){
					Instance* myFind = kid->FindFirstChild(name, recursive);
					if(myFind != NULL){
						return myFind;
					}
				}
			}
		}
		return NULL;
	}

	Instance** Instance::GetChildren(){
		return children.data();
	}

	char* Instance::GetFullName(){
		std::string fullName = Name;
		if(Parent != NULL){
			fullName = Parent->GetFullName() + fullName;
		}
		char* full = new char[fullName.size() + 1];
		std::copy(fullName.begin(), fullName.end(), full);
		full[fullName.size()] = '\0';
		return full;
	}

	bool Instance::IsA(const char* name){
		return OpenBlox::BaseGame::getInstanceFactory()->isA(this, name);
	}

	bool Instance::IsAncestorOf(Instance* descendant){
		if(descendant == NULL){
			return false;
		}
		for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
			Instance* kid = children[i];
			if(kid != NULL){
				if(kid == descendant){
					return true;
				}
			}
		}
		for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
			Instance* kid = children[i];
			if(kid != NULL){
				bool isAncestor = kid->IsAncestorOf(descendant);
				if(isAncestor){
					return true;
				}
			}
		}
		return false;
	}

	bool Instance::isDescendantOf(Instance* ancestor){
		if(ancestor == NULL){
			return false;
		}
		return ancestor->IsAncestorOf(this);
	}

	void Instance::setParent(Instance* parent){
		if(Parent != NULL){
			Parent->removeChild(this);
		}
		Parent = parent;
		if(Parent != NULL){
			Parent->addChild(this);
		}
	}

	void Instance::removeChild(Instance* kid){
		if(kid != NULL){
			std::vector<Instance*>::size_type min1 = -1;
			std::vector<Instance*>::size_type to_remove = min1;
			for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
				Instance* tkid = children[i];
				if(tkid != NULL){
					if(tkid == kid){
						to_remove = i;
						break;
					}
				}
			}
			if(to_remove != min1){
				children.erase(children.begin() + (to_remove - 1));
			}
		}
	}

	void Instance::addChild(Instance* kid){
		if(kid != NULL){
			children.push_back(kid);
		}
	}

	char* Instance::toString(){
		return ClassName;
	}

	//STATIC METHODS FOR LUA
	static int lua_Instance_toString(){

	}
}
