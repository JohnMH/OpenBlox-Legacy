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

		lua_State* L = OpenBlox::BaseGame::getGlobalState();

		luaL_newmetatable(L, LuaClassName);
		register_lua_metamethods(L);

		lua_pushstring(L, "__metatable");
		lua_pushstring(L, "This metatable is locked");
		lua_rawset(L, -3);

		// methods
		lua_pushstring(L, "__methods");
		lua_newtable(L);
		register_lua_methods(L);
		lua_rawset(L, -3);

		// property getters
		lua_pushstring(L, "__propertygetters");
		lua_newtable(L);
		register_lua_property_getters(L);
		lua_rawset(L, -3);

		// property setters
		lua_pushstring(L, "__propertysetters");
		lua_newtable(L);
		register_lua_property_setters(L);
		lua_rawset(L, -3);

		// item get
		lua_pushstring(L, "__index");
		lua_pushcfunction(L, lua_index);
		lua_rawset(L, -3);

		// item set
		lua_pushstring(L, "__newindex");
		lua_pushcfunction(L, lua_newindex);
		lua_rawset(L, -3);

		lua_pop(L, 1);
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

	void Instance::render(){

	}

	void Instance::renderChildren(){
		for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
			Instance* kid = children[i];
			if(kid != NULL){
				kid->render();
				kid->renderChildren();
			}
		}
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

	void Instance::register_lua_property_setters(lua_State* L){
		luaL_Reg properties[]{
				{"Name", lua_setName},
				{NULL, NULL}
		};
		luaL_register(L, NULL, properties);
	}

	void Instance::register_lua_property_getters(lua_State* L){
		luaL_Reg properties[]{
				{"Name", lua_getName},
				{NULL, NULL}
		};
		luaL_register(L, NULL, properties);
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
			std::vector<const char*> existing = OpenBlox::BaseGame::getInstanceFactory()->getRegisteredMetatables();
			unsigned size = existing.size();
			void* udata = lua_touserdata(L, index);
			int meta = lua_getmetatable(L, index);
			if(meta != 0){
				for(unsigned i = 0; i<size; i++){
					std::string name = std::string("luaL_Instance_") + existing[i];
					luaL_getmetatable(L, name.c_str());
					if(lua_rawequal(L, -1, -2)){
						lua_pop(L, 2);
						return *(Instance**)udata;
					}
					lua_pop(L, 1); // don't mind me just testing something
				}
			}
			return NULL;
		}
		return NULL;
	}

	int Instance::lua_newindex(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if (inst != NULL){
			const char* name = luaL_checkstring(L, 2);
			int meta = lua_getmetatable(L, 1); // -3
			lua_getfield(L, -1, "__propertysetters"); // -2
			lua_getfield(L, -1, name); // -1
			if (lua_iscfunction(L, -1)){
				lua_remove(L, -2);
				lua_remove(L, -2);

				lua_pushvalue(L, 1);
				lua_pushvalue(L, 3);
				lua_call(L, 2, 0);
				return 0;
			} else {
				lua_pop(L, 3);

				return luaL_error(L, "attempt to index '%s' (a nil value)", name);
			}
		}
		return 0;
	}

	int Instance::lua_index(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if (inst != NULL){
			const char* name = luaL_checkstring(L, 2);
			int meta = lua_getmetatable(L, 1); // -3
			lua_getfield(L, -1, "__propertygetters"); // -2
			lua_getfield(L, -1, name); // -1
			if (lua_iscfunction(L, -1)){
				lua_remove(L, -2);
				lua_remove(L, -2);

				lua_pushvalue(L, 1);
				lua_call(L, 1, 1);
				return 1;
			} else {
				lua_pop(L, 2);
				// check methods
				lua_getfield(L, -1, "__methods"); // -2
				lua_getfield(L, -1, name); // -1
				if (lua_iscfunction(L, -1)){
					lua_remove(L, -2);
					lua_remove(L, -3);

					return 1;
				} else {
					// TODO: check children
					lua_pop(L, 3);

					return luaL_error(L, "attempt to index '%s' (a nil value)", name);
				}
			}
		}
		return 0;
	}

	int Instance::lua_toString(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			lua_pushstring(L, inst->toString());
			return 1;
		}
		return 0;
	}

	//Properties
	int Instance::lua_getName(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if (inst != NULL){
			lua_pushstring(L, inst->Name);
			return 1;
		}
		return 0;
	}
	int Instance::lua_setName(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if (inst != NULL){
			std::string desired = std::string(luaL_checkstring(L, 2));
			char* newname = new char[desired.size() + 1];
			std::copy(desired.begin(), desired.end(), newname);
			newname[desired.size()] = '\0';
			inst->Name = newname;
			return 0;
		}
		std::cout<<"inst was null"<<std::endl;
		return 0;
	}

	//Methods
	int Instance::lua_ClearAllChildren(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			inst->ClearAllChildren();
			return 0;
		}
		return luaL_error(L, COLONERR, "ClearAllChildren");
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
		return luaL_error(L, COLONERR, "Clone");
	}

	int Instance::lua_Destroy(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			inst->Destroy();
			return 0;
		}
		return luaL_error(L, COLONERR, "Destroy");
	}

	int Instance::lua_Remove(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			inst->Remove();
			return 0;
		}
		return luaL_error(L, COLONERR, "Remove");
	}

	int Instance::lua_FindFirstChild(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			return 0;
		}
		return luaL_error(L, COLONERR, "FindFirstChild");
	}

	int Instance::lua_GetChildren(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			//TODO:Implement
			return 0;
		}
		return luaL_error(L, COLONERR, "GetChildren");
	}

	int Instance::lua_GetFullName(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			char* fullName = inst->GetFullName();
			lua_pushstring(L, fullName);
			return 1;
		}
		return luaL_error(L, COLONERR, "GetFullName");
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
		return luaL_error(L, COLONERR, "IsA");
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
				return luaL_typerror(L, 2, "Instance");
			}
			return 0;
		}
		return luaL_error(L, COLONERR, "IsAncestorOf");
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
				return luaL_typerror(L, 2, "Instance");
			}
			return 0;
		}
		return luaL_error(L, COLONERR, "IsDescendantOf");;
	}
}
