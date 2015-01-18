#include "RunService.h"

namespace ob_instance{
	DEFINE_CLASS(RunService, false, isDataModel);

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

	Instance* RunService::cloneImpl(){
		return NULL;
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
