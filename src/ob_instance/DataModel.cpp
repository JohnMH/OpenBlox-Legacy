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

		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, Instance::register_lua_events);
	}

	char* DataModel::ClassName = "DataModel";
	char* DataModel::LuaClassName = "luaL_Instance_DataModel";

	DataModel::DataModel() : ServiceProvider(){
		Name = ClassName;
		ParentLocked = true;
		usedInternally = true;
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

	char* DataModel::getClassName(){
		return ClassName;
	}

	void DataModel::register_lua_property_setters(lua_State* L){
		/*
		luaL_Reg properties[]{
			{NULL, NULL}
		};
		luaL_register(L, NULL, properties);
		*/
		Instance::register_lua_property_setters(L);
	}

	void DataModel::register_lua_property_getters(lua_State* L){
		/*
		luaL_Reg properties[]{
			{NULL, NULL}
		};
		luaL_register(L, NULL, properties);
		*/
		Instance::register_lua_property_getters(L);
	}

	void DataModel::register_lua_methods(lua_State* L){
		ServiceProvider::register_lua_methods(L);
	}
}
