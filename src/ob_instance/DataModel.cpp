#include "DataModel.h"

namespace ob_instance{
	struct DataModelClassMaker: public OpenBlox::ClassMaker{
		ob_instance::Instance* getInstance() const{
			return NULL;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const DataModel*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return false;
		}

		bool isService(){
			return false;
		}
	};

	STATIC_INIT(DataModel){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new DataModelClassMaker());

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

	char* DataModel::ClassName = "DataModel";
	char* DataModel::LuaClassName = "luaL_Instance_DataModel";

	DataModel::DataModel() : ServiceProvider(){
		Name = ClassName;
	}

	DataModel::~DataModel(){

	}

	void DataModel::render(){
		glLoadIdentity();
		glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

		glBegin(GL_TRIANGLES);
		{
			glColor3f(1.f, 0.f, 0.f);
			glVertex3f(-0.6f, -0.4f, 0.f);
			glColor3f(0.f, 1.f, 0.f);
			glVertex3f(0.6f, -0.4f, 0.f);
			glColor3f(0.f, 0.f, 1.f);
			glVertex3f(0.f, 0.6f, 0.f);
		}
		glEnd();
	}

	Instance* DataModel::cloneImpl(){
		return NULL;
	}

	int DataModel::wrap_lua(lua_State* L){
		Instance** udata = (Instance**)lua_newuserdata(L, sizeof(*this));
		*udata = this;

		luaL_getmetatable(L, LuaClassName);
		lua_setmetatable(L, -2);

		return 1;
	}

	void DataModel::register_lua_property_setters(lua_State* L){
		luaL_Reg properties[]{
				{NULL, NULL}
		};
		luaL_register(L, NULL, properties);
		Instance::register_lua_property_setters(L);
	}

	void DataModel::register_lua_property_getters(lua_State* L){
			luaL_Reg properties[]{
					{NULL, NULL}
			};
			luaL_register(L, NULL, properties);
			Instance::register_lua_property_getters(L);
		}

	void DataModel::register_lua_methods(lua_State* L){
		ServiceProvider::register_lua_methods(L);
	}
}
