#include "RunService.h"

namespace ob_instance{
	struct RunServiceClassMaker: public OpenBlox::ClassMaker{
		ob_instance::Instance* getInstance() const{
			return new RunService;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const RunService*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return false;
		}

		bool isService(bool isDataModel){
			return isDataModel;
		}
	};

	STATIC_INIT(RunService){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new RunServiceClassMaker());

		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
	}

	std::string RunService::ClassName = "RunService";
	std::string RunService::LuaClassName = "luaL_Instance_RunService";

	RunService::RunService() : Instance(){
		Name = ClassName;

		RenderStepped = new ob_type::LuaEvent("RenderStepped", 0);
		Stepped = new ob_type::LuaEvent("Stepped", 2);
		Heartbeat = new ob_type::LuaEvent("Heartbeat", 1);
	}

	RunService::~RunService(){
		delete RenderStepped;
		delete Heartbeat;
	}

	void RunService::Destroy(){}

	int RunService::wrap_lua(lua_State* L){
		RunService** udata = (RunService**)lua_newuserdata(L, sizeof(*this));
		*udata = this;

		luaL_getmetatable(L, LuaClassName.c_str());
		lua_setmetatable(L, -2);

		return 1;
	}

	Instance* RunService::cloneImpl(){
		return NULL;
	}

	std::string RunService::getClassName(){
		return ClassName;
	}

	void RunService::register_lua_events(lua_State* L){
		luaL_Reg events[]{
			{"RenderStepped", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(inst){
					if(RunService* rs = dynamic_cast<RunService*>(inst)){
						return rs->RenderStepped->wrap_lua(L);
					}
				}
				return 0;
			}},
			{"Stepped", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(inst){
					if(RunService* rs = dynamic_cast<RunService*>(inst)){
						return rs->Stepped->wrap_lua(L);
					}
				}
				return 0;
			}},
			{"Heartbeat", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(inst){
					if(RunService* rs = dynamic_cast<RunService*>(inst)){
						return rs->Heartbeat->wrap_lua(L);
					}
				}
				return 0;
			}},
			{NULL, NULL}
		};
		luaL_register(L, NULL, events);

		Instance::register_lua_events(L);
	}
}
