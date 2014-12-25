#include "GuiBase2d.h"

namespace ob_instance{
	struct GuiBase2dClassMaker: public OpenBlox::ClassMaker{
		ob_instance::Instance* getInstance() const{
			return NULL;
		}

		bool isA(const ob_instance::Instance* obj){
			return (dynamic_cast<const GuiBase2d*>(obj)) != 0;
		}

		bool isInstantiatable(){
			return false;
		}

		bool isService(bool isDataModel){
			return false;
		}
	};

	STATIC_INIT(GuiBase2d){
		OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new GuiBase2dClassMaker());

		registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events);
	}

	char* GuiBase2d::ClassName = "GuiBase2d";
	char* GuiBase2d::LuaClassName = "luaL_Instance_GuiBase2d";

	GuiBase2d::GuiBase2d() : GuiBase(){
		Name = ClassName;

		AbsolutePosition = new ob_type::Vector2(0, 0);
		AbsoluteSize = new ob_type::Vector2(0, 0);
	}

	GuiBase2d::~GuiBase2d(){
		if(AbsolutePosition){
			delete AbsolutePosition;
		}
		if(AbsoluteSize){
			delete AbsoluteSize;
		}
	}

	void GuiBase2d::sizeChanged(){}

	void GuiBase2d::setAbsolutePosition(ob_type::Vector2* newVec2){
		if(newVec2){
			if(AbsolutePosition == NULL || !newVec2->equals(AbsolutePosition)){
				if(AbsolutePosition){
					delete AbsolutePosition;
				}
				AbsolutePosition = newVec2->clone();

				for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
					Instance* kid = children[i];
					if(kid != NULL){
						if(GuiBase2d* sg = dynamic_cast<GuiBase2d*>(kid)){
							sg->sizeChanged();
						}
					}
				}

				Changed->Fire([](lua_State* L, va_list args){
					lua_pushstring(L, "AbsolutePosition");
				});
			}
		}
	}

	ob_type::Vector2* GuiBase2d::getAbsolutePosition(){
		if(AbsolutePosition){
			return AbsolutePosition->clone();
		}
		return NULL;
	}

	void GuiBase2d::setAbsoluteSize(ob_type::Vector2* newVec2){
		if(newVec2){
			if(AbsoluteSize == NULL || !newVec2->equals(AbsoluteSize)){
				if(AbsoluteSize){
					delete AbsoluteSize;
				}
				AbsoluteSize = newVec2->clone();

				for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
					Instance* kid = children[i];
					if(kid != NULL){
						if(GuiBase2d* sg = dynamic_cast<GuiBase2d*>(kid)){
							sg->sizeChanged();
						}
					}
				}

				Changed->Fire([](lua_State* L, va_list args){
					lua_pushstring(L, "AbsoluteSize");
				});
			}
		}
	}

	ob_type::Vector2* GuiBase2d::getAbsoluteSize(){
		if(AbsoluteSize){
			return AbsoluteSize->clone();
		}
		return NULL;
	}

	char* GuiBase2d::getClassName(){
		return ClassName;
	}

	int GuiBase2d::lua_getAbsolutePosition(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst){
			if(GuiBase2d* gb2d = dynamic_cast<GuiBase2d*>(inst)){
				if(gb2d->AbsolutePosition){
					return gb2d->AbsolutePosition->wrap_lua(L);
				}else{
					lua_pushnil(L);
					return 1;
				}
			}
		}
		return 0;
	}

	int GuiBase2d::lua_getAbsoluteSize(lua_State* L){
		Instance* inst = checkInstance(L, 1);
		if(inst){
			if(GuiBase2d* gb2d = dynamic_cast<GuiBase2d*>(inst)){
				if(gb2d->AbsoluteSize){
					return gb2d->AbsoluteSize->wrap_lua(L);
				}else{
					lua_pushnil(L);
					return 1;
				}
			}
		}
		return 0;
	}

	void GuiBase2d::register_lua_property_setters(lua_State* L){
		luaL_Reg properties[]{
			{"AbsolutePosition", lua_readOnlyProperty},
			{"AbsoluteSize", lua_readOnlyProperty},
			{NULL, NULL}
		};
		luaL_register(L, NULL, properties);

		Instance::register_lua_property_setters(L);
	}

	void GuiBase2d::register_lua_property_getters(lua_State* L){
		luaL_Reg properties[]{
			{"AbsolutePosition", lua_getAbsolutePosition},
			{"AbsoluteSize", lua_getAbsoluteSize},
			{NULL, NULL}
		};
		luaL_register(L, NULL, properties);

		Instance::register_lua_property_getters(L);
	}
}
