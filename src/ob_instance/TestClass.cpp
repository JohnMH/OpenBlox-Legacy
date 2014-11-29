#include "TestClass.h"

namespace ob_instance{
	struct TestClassClassMaker: public OpenBlox::ClassMaker{
		ob_instance::Instance* getInstance() const{
			return new TestClass();
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const TestClass*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return true;
		}

		bool isService(){
			return false;
		}
	};

	STATIC_INIT(TestClass){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new TestClassClassMaker());

		lua_State* L = OpenBlox::BaseGame::getGlobalState();

		luaL_newmetatable(L, LuaClassName);

		register_lua_metamethods(L);

		lua_pushstring(L, "__index");
		lua_newtable(L);

		lua_pushstring(L, "ClassName");
		lua_pushstring(L, ClassName);
		lua_rawset(L, -3);

		register_lua_methods(L);

		lua_rawset(L, -3);
		lua_pop(L, 1);
	}

	char* TestClass::ClassName = "TestClass";
	char* TestClass::LuaClassName = "luaL_Instance_TestClass";

	TestClass::TestClass() : Instance(){
		Name = ClassName;
	}

	TestClass::~TestClass(){

	}

	int TestClass::wrap_lua(lua_State* L){
		TestClass** udata = (TestClass**)lua_newuserdata(L, sizeof(TestClass*));
		*udata = this;

		luaL_getmetatable(L, LuaClassName);
		lua_setmetatable(L, -2);

		return 1;
	}

	Instance* TestClass::cloneImpl(){
		TestClass* newGuy = new TestClass();
		newGuy->Name = Name;
		return newGuy;
	}
}
