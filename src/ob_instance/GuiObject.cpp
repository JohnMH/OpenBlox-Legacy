#include "GuiObject.h"

namespace ob_instance{
	DEFINE_ABS_CLASS(GuiObject);

	GuiObject::GuiObject() : GuiBase2d(){
		Name = ClassName;

		Active = true;
		BackgroundColor3 = new ob_type::Color3(1, 1, 1);
		BackgroundTransparency = 0;
		BorderColor3 = new ob_type::Color3();
		BorderSizePixel = 1;
		ClipsDescendants = true;
		Draggable = false;
		Position = new ob_type::UDim2(0, 0, 0, 0);
		Rotation = 0;
		Size = new ob_type::UDim2(0, 100, 0, 100);
		Visible = true;
		ZIndex = 1;

		DragBegin = new ob_type::LuaEvent("DragBegin", 1);
		DragStopped = new ob_type::LuaEvent("DragStopped", 2);

		InputBegan = new ob_type::LuaEvent("InputBegan", 1);
		InputChanged = new ob_type::LuaEvent("InputChanged", 1);
		InputEnded = new ob_type::LuaEvent("InputEnded", 1);

		MouseEnter = new ob_type::LuaEvent("MouseEnter", 2);
		MouseLeave = new ob_type::LuaEvent("MouseLeave", 2);
		MouseMoved = new ob_type::LuaEvent("MouseMoved", 2);
		MouseWheelBackward = new ob_type::LuaEvent("MouseWheelBackward", 2);
		MouseWheelForward = new ob_type::LuaEvent("MouseWheelForward", 2);

		TouchLongPress = new ob_type::LuaEvent("TouchLongPress", 2);
		TouchPan = new ob_type::LuaEvent("TouchPan", 4);
		TouchPinch = new ob_type::LuaEvent("TouchPinch", 4);
		TouchRotate = new ob_type::LuaEvent("TouchRotate", 4);
		TouchSwipe = new ob_type::LuaEvent("TouchPan", 2);
		TouchTap = new ob_type::LuaEvent("TouchPan", 1);
	}

	GuiObject::~GuiObject(){
		delete DragBegin;
		delete DragStopped;

		delete InputBegan;
		delete InputChanged;
		delete InputEnded;

		delete MouseEnter;
		delete MouseLeave;
		delete MouseMoved;
		delete MouseWheelBackward;
		delete MouseWheelForward;

		delete TouchLongPress;
		delete TouchPan;
		delete TouchPinch;
		delete TouchRotate;
		delete TouchSwipe;
		delete TouchTap;
	}

	Instance* GuiObject::cloneImpl(){
		return NULL;
	}

	void GuiObject::sizeChanged(){
		ob_type::Vector2* newPos = calculateRenderPosision(new ob_type::Vector2());
		if(newPos){
			setAbsolutePosition(newPos);
			delete newPos;
		}
		ob_type::Vector2* newSize = calculateRenderSize(new ob_type::Vector2());
		if(newSize){
			setAbsoluteSize(newSize);
			delete newSize;
		}
	}

	bool GuiObject::isMouseCaptured(int x, int y){
		return false;//Might wanna.. Y'know...... Implement this.
	}

	ob_type::Vector2* GuiObject::calculateRenderPosision(ob_type::Vector2* seed){
		if(Parent){
			if(GuiBase2d* par = dynamic_cast<GuiBase2d*>(Parent)){
				seed->x = par->AbsolutePosition->x + Position->x->offset + (Position->x->scale * par->AbsoluteSize->x);
				seed->y = par->AbsolutePosition->y + Position->y->offset + (Position->y->scale * par->AbsoluteSize->y);
				return seed;
			}
		}
		return seed;
	}

	ob_type::Vector2* GuiObject::calculateRenderSize(ob_type::Vector2* seed){
		if(Parent){
			if(GuiBase2d* par = dynamic_cast<GuiBase2d*>(Parent)){
				seed->x = Size->x->offset + (par->AbsoluteSize->x * Size->x->scale);
				seed->y = Size->y->offset + (par->AbsoluteSize->y * Size->y->scale);
				return seed;
			}
		}
		return seed;
	}

	void GuiObject::giveUpKids(std::vector<GuiObject*>* kids){
		if(Visible && Active){
			kids->push_back(this);

			for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
				Instance* kid = children[i];
				if(kid != NULL){
					if(GuiObject* go = dynamic_cast<GuiObject*>(kid)){
						go->giveUpKids(kids);
					}
				}
			}
		}
	}

	ob_type::Vector2* rotVec2(ob_type::Vector2* p, double rot){
		double s = sin(rot);
		double c = cos(rot);

		double nx = 0;
		double ny = 0;

		nx -= p->x;
		ny -= p->y;

		nx = nx * c + ny * s;
		ny = -nx * s + ny * c;

		nx = nx + p->x;
		ny = ny + p->y;

		return new ob_type::Vector2(nx, ny);
	}

	bool GuiObject::containsPoint(int x, int y){
		double rot = Rotation;
		rot = (M_PI * rot) / 180;

		ob_type::Vector2* ap = AbsolutePosition;
		ob_type::Vector2* as = AbsoluteSize;

		//ap = rotVec2(ap, rot);
		//as = rotVec2(as, rot);

		ob_type::Vector2* p2 = new ob_type::Vector2(x, y);
		//ob_type::Vector2* p2 = rotVec2(point, rot);

		bool isIn = false;

		if(p2->x < (ap->x + (.5*as->x)) && p2->x > (ap->x - (.5*as->x)) && p2->y < (ap->y + (.5*as->y)) && p2->y > (ap->y - (.5*as->y))){
			isIn = true;
		}

		//delete ap;
		//delete as;
		//delete point;
		delete p2;

		return isIn;
	}

	void GuiObject::register_lua_property_setters(lua_State* L){
		luaL_Reg properties[]{
			{"Active", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					bool newVal = false;
					if(lua_isboolean(L, 2)){
						newVal = lua_toboolean(L, 2);
					}else if(!lua_isnoneornil(L, 2)){
						newVal = true;
					}
					if(go->Active != newVal){
						go->Active = newVal;

						std::vector<ob_type::VarWrapper> args = std::vector<ob_type::VarWrapper>();
						propertyChanged("Active", go);
					}
				}
				return 0;
			}},
			{"BackgroundColor3", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					ob_type::Color3* newVal = ob_type::checkColor3(L, 2);
					if(go->BackgroundColor3 != newVal){
						go->BackgroundColor3 = newVal;

						propertyChanged("BackgroundColor3", go);
					}
				}
				return 0;
			}},
			{"BackgroundTransparency", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					double newVal = luaL_checknumber(L, 2);
					if(go->BackgroundTransparency != newVal){
						go->BackgroundTransparency = newVal;

						propertyChanged("BackgroundTransparency", go);
					}
				}
				return 0;
			}},
			{"BorderColor3", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					ob_type::Color3* newVal = ob_type::checkColor3(L, 2);
					if(go->BorderColor3 != newVal){
						go->BorderColor3 = newVal;

						propertyChanged("BorderColor3", go);
					}
				}
				return 0;
			}},
			{"BorderSizePixel", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					int newVal = luaL_checkint(L, 2);
					if(go->BorderSizePixel != newVal){
						go->BorderSizePixel = newVal;

						propertyChanged("BorderSizePixel", go);
					}
				}
				return 0;
			}},
			{"ClipsDescendants", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					bool newVal = false;
					if(lua_isboolean(L, 2)){
						newVal = lua_toboolean(L, 2);
					}else if(!lua_isnoneornil(L, 2)){
						newVal = true;
					}
					if(go->ClipsDescendants != newVal){
						go->ClipsDescendants = newVal;

						propertyChanged("ClipsDescendants", go);
					}
				}
				return 0;
			}},
			{"Draggable", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					bool newVal = false;
					if(lua_isboolean(L, 2)){
						newVal = lua_toboolean(L, 2);
					}else if(!lua_isnoneornil(L, 2)){
						newVal = true;
					}
					if(go->Draggable != newVal){
						go->Draggable = newVal;

						propertyChanged("Draggable", go);
					}
				}
				return 0;
			}},
			{"Position", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					ob_type::UDim2* newVal = ob_type::checkUDim2(L, 2);
					if(go->Position != newVal){
						go->Position = newVal;

						go->sizeChanged();

						propertyChanged("Position", go);
					}
				}
				return 0;
			}},
			{"Rotation", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					double newVal = luaL_checknumber(L, 2);
					if(go->Rotation != newVal){
						go->Rotation = newVal;

						propertyChanged("Rotation", go);
					}
				}
				return 0;
			}},
			{"Size", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					ob_type::UDim2* newVal = ob_type::checkUDim2(L, 2);
					if(go->Size != newVal){
						go->Size = newVal;

						go->sizeChanged();

						propertyChanged("Size", go);
					}
				}
				return 0;
			}},
			{"Visible", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					bool newVal = false;
					if(lua_isboolean(L, 2)){
						newVal = lua_toboolean(L, 2);
					}else if(!lua_isnoneornil(L, 2)){
						newVal = true;
					}
					if(go->Visible != newVal){
						go->Visible = newVal;

						propertyChanged("Visible", go);
					}
				}
				return 0;
			}},
			{"ZIndex", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					int newVal = luaL_checkint(L, 2);
					if(go->ZIndex != newVal){
						go->ZIndex = newVal;

						propertyChanged("Visible", go);
					}
				}
				return 0;
			}},
			{NULL, NULL}
		};
		luaL_register(L, NULL, properties);

		GuiBase2d::register_lua_property_setters(L);
	}

	void GuiObject::register_lua_property_getters(lua_State* L){
		luaL_Reg properties[]{
			{"Active", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					lua_pushboolean(L, go->Active);
					return 1;
				}
				return 0;
			}},
			{"BackgroundColor3", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					return go->BackgroundColor3->wrap_lua(L);
				}
				return 0;
			}},
			{"BackgroundTransparency", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					lua_pushnumber(L, go->BackgroundTransparency);
					return 1;
				}
				return 0;
			}},
			{"BorderColor3", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					return go->BorderColor3->wrap_lua(L);
				}
				return 0;
			}},
			{"BorderSizePixel", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					lua_pushnumber(L, go->BorderSizePixel);
					return 1;
				}
				return 0;
			}},
			{"ClipsDescendants", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					lua_pushboolean(L, go->ClipsDescendants);
					return 1;
				}
				return 0;
			}},
			{"Draggable", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					lua_pushboolean(L, go->Draggable);
					return 1;
				}
				return 0;
			}},
			{"Position", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					return go->Position->wrap_lua(L);
				}
				return 0;
			}},
			{"Rotation", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					lua_pushnumber(L, go->Rotation);
					return 1;
				}
				return 0;
			}},
			{"Size", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					return go->Size->wrap_lua(L);
				}
				return 0;
			}},
			{"Visible", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					lua_pushboolean(L, go->Visible);
					return 1;
				}
				return 0;
			}},
			{"ZIndex", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(GuiObject* go = dynamic_cast<GuiObject*>(inst)){
					lua_pushnumber(L, go->ZIndex);
					return 1;
				}
				return 0;
			}},
			{NULL, NULL}
		};
		luaL_register(L, NULL, properties);

		GuiBase2d::register_lua_property_getters(L);
	}

	void GuiObject::register_lua_methods(lua_State* L){
		luaL_Reg methods[]{
			{NULL, NULL}
		};
		luaL_register(L, NULL, methods);

		Instance::register_lua_methods(L);
	}

	void GuiObject::register_lua_events(lua_State* L){
		Instance::register_lua_events(L);
	}
}
