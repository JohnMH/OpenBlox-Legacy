#include "Instance.h"

namespace ob_instance{
	struct InstanceClassMaker: public OpenBlox::ClassMaker{
		ob_instance::Instance* getInstance() const{
			return NULL;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const Instance*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return false;
		}

		bool isService(){
			return false;
		}
	};

	STATIC_INIT(Instance){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new InstanceClassMaker());
	}

	char* Instance::ClassName = "Instance";
	char* Instance::LuaClassName = "luaL_Instance_Instance";

	Instance::Instance(){
		Archivable = true;
		Name = ClassName;
		Parent = NULL;
		ParentLocked = false;

		children = std::vector<Instance*>();
	}

	Instance::~Instance(){
		delete[] Name;
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
		ParentLocked = true;
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
			return true;
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

	bool Instance::IsDescendantOf(Instance* ancestor){
		if(ancestor == NULL){
			return false;
		}
		return ancestor->IsAncestorOf(this);
	}

	char* Instance::toString(){
		return Name;
	}

	void Instance::setParent(Instance* parent){
		if(ParentLocked){
			std::string errMsg = "The Parent property of ";
			errMsg = errMsg + Name + " is locked.";
			throw std::runtime_error(errMsg);
			return;
		}
		if(parent == this){
			std::string errMsg = "Attempt to set ";
			errMsg = errMsg + GetFullName() + " as its own parent";
			throw std::runtime_error(errMsg);
			return;
		}
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

	void Instance::register_lua_metamethods(lua_State* L){
		luaL_Reg metamethods[] = {
			{"__tostring", Instance::lua_toString},
			{NULL, NULL}
		};
		luaL_register(L, NULL, metamethods);
	}

	void Instance::register_lua_methods(lua_State* L){
		luaL_Reg methods[]{
			{"ClearAllChildren", lua_ClearAllChildren},
			{"Clone", lua_Clone},
			{"Destroy", lua_Destroy},
			{"Remove", lua_Remove},
			{"FindFirstChild", lua_FindFirstChild},
			{"GetChildren", lua_GetChildren},
			{"GetFullName", lua_GetFullName},
			{"IsA", lua_IsA},
			{"IsAncestorOf", lua_IsAncestorOf},
			{"IsDescendantOf", lua_IsDescendantOf},
			{NULL, NULL}
		};
		luaL_register(L, NULL, methods);
	}

	//Lua Wrappers
	//Metamethods
	Instance* Instance::checkInstance(lua_State* L, int index){
		if(lua_isuserdata(L, index)){
			Instance* inst = *(Instance**)lua_touserdata(L, index);
			/*
			try{
				if(Instance* ins = dynamic_cast<Instance*>(inst)){
					std::cout << "Is Instance" << std::endl;
					return ins;
				}
			}catch(std::exception& e){
				std::cout << "Bad Cast" << std::endl;
			}
			*/
			//TODO: Actually do typechecking. Trust me, we don't want C++ calling stuff on random userdata.
			return inst;
		}
		return NULL;
	}

	int Instance::lua_toString(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			lua_pushstring(L, inst->toString());
			return 1;
		}
		return 0;
	}

	//Methods
	int Instance::lua_ClearAllChildren(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			inst->ClearAllChildren();
			return 0;
		}
		lua_pushstring(L,"Expected ':' not '.' calling member function ClearAllChildren");
		lua_error(L);
		return 0;
	}

	int Instance::lua_Clone(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			Instance* newGuy = inst->Clone();
			if(newGuy != NULL){
				return newGuy->wrap_lua(L);
			}
			return 0;
		}
		lua_pushstring(L,"Expected ':' not '.' calling member function Clone");
		lua_error(L);
		return 0;
	}

	int Instance::lua_Destroy(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			inst->Destroy();
			return 0;
		}
		lua_pushstring(L,"Expected ':' not '.' calling member function Destroy");
		lua_error(L);
		return 0;
	}

	int Instance::lua_Remove(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			inst->Remove();
			return 0;
		}
		lua_pushstring(L,"Expected ':' not '.' calling member function Remove");
		lua_error(L);
		return 0;
	}

	int Instance::lua_FindFirstChild(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			return 0;
		}
		lua_pushstring(L,"Expected ':' not '.' calling member function FindFirstChild");
		lua_error(L);
		return 0;
	}

	int Instance::lua_GetChildren(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			//TODO:Implement
			return 0;
		}
		lua_pushstring(L,"Expected ':' not '.' calling member function GetChildren");
		lua_error(L);
		return 0;
	}

	int Instance::lua_GetFullName(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			char* fullName = inst->GetFullName();
			lua_pushstring(L, fullName);
			return 1;
		}
		lua_pushstring(L,"Expected ':' not '.' calling member function GetFullName");
		lua_error(L);
		return 0;
	}

	int Instance::lua_IsA(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			const char* checkName = luaL_checkstring(L, 2);
			if(checkName != NULL){
				bool isIt = inst->IsA(checkName);
				lua_pushboolean(L, isIt);
				return 1;
			}
			lua_pushboolean(L, false);
			return 1;
		}
		lua_pushstring(L,"Expected ':' not '.' calling member function IsA");
		lua_error(L);
		return 0;
	}

	int Instance::lua_IsAncestorOf(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			bool throwErrorIf = true;
			Instance* otherInst = NULL;
			if(lua_isnil(L, 2)){
				throwErrorIf = false;
			}else{
				otherInst = checkInstance(L, 2);
			}
			if(otherInst != NULL || !throwErrorIf){
				bool isIt = inst->IsAncestorOf(otherInst);
				lua_pushboolean(L, isIt);
				return 1;
			}else{
				return luaL_typerror(L, 2, "Instance");//TODO: Keep using "Instance" or switch to "userdata"?
			}
			return 0;
		}
		lua_pushstring(L,"Expected ':' not '.' calling member function IsAncestorOf");
		return lua_error(L);
	}

	int Instance::lua_IsDescendantOf(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			bool throwErrorIf = true;
			Instance* otherInst = NULL;
			if(lua_isnil(L, 2)){
				throwErrorIf = false;
			}else{
				otherInst = checkInstance(L, 2);
			}
			if(otherInst != NULL || !throwErrorIf){
				bool isIt = inst->IsDescendantOf(otherInst);
				lua_pushboolean(L, isIt);
				return 1;
			}else{
				return luaL_typerror(L, 2, "Instance");//TODO: Keep using "Instance" or switch to "userdata"?
			}
			return 0;
		}
		lua_pushstring(L,"Expected ':' not '.' calling member function DescendantOf");
		return lua_error(L);
	}
}
