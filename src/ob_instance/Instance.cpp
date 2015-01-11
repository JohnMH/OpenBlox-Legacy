#include "Instance.h"

#include "../openblox/OpenBloxRenderUtil.h"

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

		bool isService(bool isDataModel){
			return false;
		}
	};

	STATIC_INIT(Instance){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new InstanceClassMaker());

		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
	}

	std::string Instance::ClassName = "Instance";
	std::string Instance::LuaClassName = "luaL_Instance_Instance";

	Instance::Instance(){
		Archivable = true;
		Name = ClassName;
		Parent = NULL;
		ParentLocked = false;

		children = std::vector<Instance*>();
		Changed = new ob_type::LuaEvent("Changed", 1);
	}

	Instance::~Instance(){
		delete Changed;
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

	void Instance::parentLock(){
		ParentLocked = true;
	}

	void Instance::Destroy(){
		setParent(NULL);
		ParentLocked = true;
		Changed->disconnectAll();
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

	Instance* Instance::FindFirstChild(std::string name, bool recursive){
		for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
			Instance* kid = children[i];
			if(kid != NULL){
				if(kid->Name == name){
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

	std::string Instance::GetFullName(){
		std::string fullName = Name;
		if(Parent != NULL){
			fullName = Parent->GetFullName() + fullName;
		}
		return fullName;
	}

	bool Instance::IsA(std::string name){
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

	void Instance::serialize_impl(rapidjson::Writer<rapidjson::StringBuffer>* writer){
		writer->String("ClassName");
		writer->String(getClassName().c_str());

		writer->String("Name");
		writer->String(Name.c_str());

		writer->String("Parent");
		writer->String("DEBUG_NULL");//TODO: Instance DebugID

		writer->String("Archivable");
		writer->Bool(Archivable);
	}

	void Instance::serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer){
		writer->StartObject();

		serialize_impl(writer);

		writer->EndObject();
	}

	std::string Instance::getClassName(){
		return ClassName;
	}

	void Instance::render(){}

	void Instance::renderChildren(){
		for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
			Instance* kid = children[i];
			if(kid != NULL){
				kid->render();
				kid->renderChildren();
			}
		}
	}

	void Instance::registerLuaClass(std::string className, luaRegisterFunc register_metamethods, luaRegisterFunc register_methods, luaRegisterFunc register_getters, luaRegisterFunc register_setters, luaRegisterFunc register_events){
		lua_State* L = OpenBlox::BaseGame::getGlobalState();

		luaL_newmetatable(L, className.c_str());
		register_metamethods(L);

		lua_pushstring(L, "__metatable");
		lua_pushstring(L, "This metatable is locked");
		lua_rawset(L, -3);

		//Methods
		lua_pushstring(L, "__methods");
		lua_newtable(L);
		register_methods(L);
		lua_rawset(L, -3);

		//Property getters
		lua_pushstring(L, "__propertygetters");
		lua_newtable(L);
		register_getters(L);
		lua_rawset(L, -3);

		//Property setters
		lua_pushstring(L, "__propertysetters");
		lua_newtable(L);
		register_setters(L);
		lua_rawset(L, -3);

		//Events
		lua_pushstring(L, "__events");
		lua_newtable(L);
		register_events(L);
		lua_rawset(L, -3);

		//Item get
		lua_pushstring(L, "__index");
		lua_pushcfunction(L, lua_index);
		lua_rawset(L, -3);

		//Item set
		lua_pushstring(L, "__newindex");
		lua_pushcfunction(L, lua_newindex);
		lua_rawset(L, -3);

		lua_pop(L, 1);
	}

	std::string Instance::toString(){
		//return Name;
		rapidjson::StringBuffer s;
		rapidjson::Writer<rapidjson::StringBuffer> writer(s);

		serialize(&writer);

		return s.GetString();
	}

	void Instance::setParent(Instance* parent){
		if(ParentLocked){
			std::string errMsg = "The Parent property of ";
			errMsg = errMsg + Name + " is locked.";
			throw std::runtime_error(errMsg.c_str());
			return;
		}
		if(parent == this){
			std::string errMsg = "Attempt to set ";
			errMsg = errMsg + GetFullName() + " as its own parent";
			throw std::runtime_error(errMsg.c_str());
			return;
		}

		if(OpenBlox::renderLock){
			while(OpenBlox::renderLock){}
		}

		if(Parent != NULL){
			Parent->removeChild(this);
		}
		Parent = parent;
		if(Parent != NULL){
			Parent->addChild(this);
		}

		std::vector<ob_type::VarWrapper> args = std::vector<ob_type::VarWrapper>();
		args.push_back(ob_type::VarWrapper("Parent"));

		Changed->Fire(args);
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
			{"__eq", lua_eq},
			{NULL, NULL}
		};
		luaL_register(L, NULL, metamethods);
	}

	void Instance::register_lua_property_setters(lua_State* L){
		luaL_Reg properties[]{
			{"ClassName", lua_readOnlyProperty},
			{"Name", lua_setName},
			{"Parent", lua_setParent},
			{"Archivable", lua_setArchivable},
			{NULL, NULL}
		};
		luaL_register(L, NULL, properties);
	}

	void Instance::register_lua_property_getters(lua_State* L){
		luaL_Reg properties[]{
			{"ClassName", lua_getClassName},
			{"Name", lua_getName},
			{"Parent", lua_getParent},
			{"Archivable", lua_getArchivable},
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

	void Instance::register_lua_events(lua_State* L){
		luaL_Reg events[]{
			{"Changed", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(inst){
					return inst->Changed->wrap_lua(L);
				}
				return 0;
			}},
			{NULL, NULL}
		};
		luaL_register(L, NULL, events);
	}

	//Lua Wrappers
	//Metamethods
	Instance* Instance::checkInstance(lua_State* L, int index){
		if(lua_isuserdata(L, index)){
			std::vector<std::string> existing = OpenBlox::BaseGame::getInstanceFactory()->getRegisteredMetatables();
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
					lua_pop(L, 1);
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
			lua_getmetatable(L, 1);//-3
			lua_getfield(L, -1, "__propertysetters");//-2
			lua_getfield(L, -1, name);//-1
			if (lua_iscfunction(L, -1)){
				lua_remove(L, -2);
				lua_remove(L, -2);

				lua_pushvalue(L, 1);
				lua_pushvalue(L, 3);
				lua_call(L, 2, 0);

				return 0;
			}else{
				lua_pop(L, 3);

				return luaL_error(L, "attempt to index '%s' (a nil value)", name);
			}
		}
		return 0;
	}

	int Instance::lua_index(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst != NULL){
			const char* name = luaL_checkstring(L, 2);

			lua_getmetatable(L, 1);//-3
			lua_getfield(L, -1, "__propertygetters");//-2
			lua_getfield(L, -1, name);//-1
			if(lua_iscfunction(L, -1)){
				lua_remove(L, -2);
				lua_remove(L, -2);

				lua_pushvalue(L, 1);
				lua_call(L, 1, 1);
				return 1;
			}else{
				lua_pop(L, 2);
				//Check methods
				lua_getfield(L, -1, "__methods");//-2
				lua_getfield(L, -1, name);//-1
				if(lua_iscfunction(L, -1)){
					lua_remove(L, -2);
					lua_remove(L, -3);

					return 1;
				}else{
					lua_pop(L, 2);
					//Check events
					lua_getfield(L, -1, "__events");//-2
					lua_getfield(L, -1, name);//-1
					if(lua_iscfunction(L, -1)){
						lua_remove(L, -2);
						lua_remove(L, -3);

						lua_pushvalue(L, 1);
						lua_call(L, 1, 1);
						return 1;
					}else{
						lua_pop(L, 3);

						Instance* kiddie = inst->FindFirstChild(name, false);
						if(kiddie){
							return kiddie->wrap_lua(L);
						}

						return luaL_error(L, "attempt to index '%s' (a nil value)", name);
					}
				}
			}
		}
		return 0;
	}

	int Instance::lua_eq(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst){
			Instance* oinst = checkInstance(L, 2);
			if(oinst){
				lua_pushboolean(L, inst == oinst);
				return 1;
			}
		}
		lua_pushboolean(L, false);
		return 1;
	}

	int Instance::lua_toString(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst){
			lua_pushstring(L, inst->toString().c_str());
			return 1;
		}
		return 0;
	}

	//Properties
	int Instance::lua_getClassName(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst){
			std::string className = inst->getClassName();
			lua_pushstring(L, className.c_str());
			return 1;
		}
		lua_pushnil(L);
		return 1;
	}

	int Instance::lua_readOnlyProperty(lua_State* L){
		//Welp. This is how ROBLOX does it.
		luaL_error(L, "can't set value");
		return 0;
	}

	int Instance::lua_getName(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst){
			lua_pushstring(L, inst->Name.c_str());
			return 1;
		}
		return 0;
	}

	int Instance::lua_setName(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst){
			std::string desired = std::string(luaL_checkstring(L, 2));
			if(inst->Name != desired){
				inst->Name = desired;

				std::vector<ob_type::VarWrapper> args = std::vector<ob_type::VarWrapper>();
				args.push_back(ob_type::VarWrapper("Name"));

				inst->Changed->Fire(args);
			}
			return 0;
		}
		return 0;
	}

	int Instance::lua_getParent(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst){
			if(inst->Parent != NULL){
				return inst->Parent->wrap_lua(L);
			}
			lua_pushnil(L);
			return 1;
		}
		return 0;
	}

	int Instance::lua_setParent(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst){
			bool throwErrorIf = true;
			Instance* otherInst = NULL;
			if(lua_isnil(L, 2)){
				throwErrorIf = false;
			}else{
				otherInst = checkInstance(L, 2);
			}
			if(otherInst != NULL || !throwErrorIf){
				try{
					inst->setParent(otherInst);
				}catch(std::runtime_error& ex){
					return luaL_error(L, ex.what());
				}
				return 0;
			}else{
				return luaL_typerror(L, 2, "Instance");
			}
			return 0;
		}
		return 0;
	}

	int Instance::lua_getArchivable(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst){
			lua_pushboolean(L, inst->Archivable);
			return 1;
		}
		return 0;
	}

	int Instance::lua_setArchivable(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst){
			//Again, following ROBLOX's ways....
			bool newVal = false;
			if(lua_isboolean(L, 2)){
				newVal = lua_toboolean(L, 2);
			}else if(!lua_isnoneornil(L, 2)){
				newVal = true;
			}
			if(inst->Archivable != newVal){
				inst->Archivable = newVal;

				std::vector<ob_type::VarWrapper> args = std::vector<ob_type::VarWrapper>();
				args.push_back(ob_type::VarWrapper("Archivable"));

				inst->Changed->Fire(args);
			}
		}
		return 0;
	}

	//Methods
	int Instance::lua_ClearAllChildren(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst){
			inst->ClearAllChildren();
			return 0;
		}
		return luaL_error(L, COLONERR, "ClearAllChildren");
	}

	int Instance::lua_Clone(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst){
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
		if(inst){
			inst->Destroy();
			return 0;
		}
		return luaL_error(L, COLONERR, "Destroy");
	}

	int Instance::lua_Remove(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst){
			inst->Remove();
			return 0;
		}
		return luaL_error(L, COLONERR, "Remove");
	}

	int Instance::lua_FindFirstChild(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst){
			const char* kidName = luaL_checkstring(L, 2);
			bool recursive = false;
			if(!lua_isnoneornil(L, 3)){
				if(lua_isboolean(L, 3)){
					recursive = lua_toboolean(L, 3);
				}else{
					luaL_typerror(L, 3, "boolean");
				}
			}
			Instance* foundStuff = inst->FindFirstChild(kidName, recursive);
			if(foundStuff != NULL){
				return foundStuff->wrap_lua(L);
			}
			lua_pushnil(L);
			return 1;
		}
		return luaL_error(L, COLONERR, "FindFirstChild");
	}

	int Instance::lua_GetChildren(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst){
			lua_newtable(L);

			for(std::vector<Instance*>::size_type i = 0; i != inst->children.size(); i++){
				Instance* kid = inst->children[i];
				if(kid != NULL){
					int lIndex = i + 1;
					//lua_pushnumber(L, lIndex);
					kid->wrap_lua(L);
					lua_rawseti(L, -2, lIndex);
				}
			}
			return 1;
		}
		return luaL_error(L, COLONERR, "GetChildren");
	}

	int Instance::lua_GetFullName(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst){
			std::string fullName = inst->GetFullName();
			lua_pushstring(L, fullName.c_str());
			return 1;
		}
		return luaL_error(L, COLONERR, "GetFullName");
	}

	int Instance::lua_IsA(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst){
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
		if(inst){
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
		if(inst){
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
