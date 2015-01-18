#ifndef OB_INSTANCE_INSTANCE_H_
#define OB_INSTANCE_INSTANCE_H_

#include "../openblox/OpenBlox.h"

#include "constants.h"

#include "../ob_type/LuaEvent.h"

#include "../ob_enum/Enums.h"

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

namespace ob_instance{
	class Instance;

	#define DECLARE_CLASS(Class_Name) \
		virtual Instance* cloneImpl(); \
		virtual std::string getClassName(); \
		virtual int wrap_lua(lua_State* L); \
		DECLARE_STATIC_INIT(Class_Name); \
		protected: \
			static std::string ClassName; \
			static std::string LuaClassName

	#define DEFINE_CLASS(Class_Name, isInstable, isAService) \
		struct Class_Name##ClassMaker: public OpenBlox::ClassMaker{ \
			ob_instance::Instance* getInstance() const{ \
				return new Class_Name; \
			} \
			bool isA(const ob_instance::Instance* obj){ \
				return (dynamic_cast<const Class_Name*>(obj)) != 0; \
			} \
			bool isInstantiatable(){ \
				return isInstable; \
			} \
			bool isService(bool isDataModel){ \
				return isAService; \
			} \
		}; \
		STATIC_INIT(Class_Name){ \
			OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new Class_Name##ClassMaker()); \
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events); \
		} \
		std::string Class_Name::ClassName = #Class_Name; \
		std::string Class_Name::LuaClassName = "luaL_Instance_" + ClassName; \
		int Class_Name::wrap_lua(lua_State* L){ \
			Class_Name** udata = (Class_Name**)lua_newuserdata(L, sizeof(*this)); \
			*udata = this; \
			luaL_getmetatable(L, LuaClassName.c_str()); \
			lua_setmetatable(L, -2); \
			return 1; \
		} \
		std::string Class_Name::getClassName(){ \
			return ClassName; \
		}

	#define DEFINE_ABS_CLASS(Class_Name) \
		struct Class_Name##ClassMaker: public OpenBlox::ClassMaker{ \
			ob_instance::Instance* getInstance() const{ \
				return NULL; \
			} \
			bool isA(const ob_instance::Instance* obj){ \
				return (dynamic_cast<const Class_Name*>(obj)) != 0; \
			} \
			bool isInstantiatable(){ \
				return false; \
			} \
			bool isService(bool isDataModel){ \
				return false; \
			} \
		}; \
		STATIC_INIT(Class_Name){ \
			OpenBlox::BaseGame::getInstanceFactory()->addClass(ClassName, new Class_Name##ClassMaker()); \
			registerLuaClass(LuaClassName, register_lua_metamethods, register_lua_methods, register_lua_property_getters, register_lua_property_setters, register_lua_events); \
		} \
		std::string Class_Name::ClassName = #Class_Name; \
		std::string Class_Name::LuaClassName = "luaL_Instance_" + ClassName; \
		int Class_Name::wrap_lua(lua_State* L){ \
			Class_Name** udata = (Class_Name**)lua_newuserdata(L, sizeof(*this)); \
			*udata = this; \
			luaL_getmetatable(L, LuaClassName.c_str()); \
			lua_setmetatable(L, -2); \
			return 1; \
		} \
		std::string Class_Name::getClassName(){ \
			return ClassName; \
		}

	class Instance{
		public:
			Instance();
			virtual ~Instance();

			ob_type::LuaEvent* Changed;

			virtual void ClearAllChildren();
			virtual Instance* Clone();
			virtual void Destroy();
			virtual void Remove();
			virtual Instance* FindFirstChild(std::string name, bool recursive = false);
			virtual Instance** GetChildren();
			virtual std::string GetFullName();
			virtual bool IsA(std::string name);
			virtual bool IsAncestorOf(Instance* descendant);
			virtual bool IsDescendantOf(Instance* ancestor);
			//virtual Instance* WaitForChild(char* childName);

			virtual void serialize_impl(rapidjson::Writer<rapidjson::StringBuffer>* writer);
			virtual void serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer);

			void parentLock();

			virtual std::string getClassName();

			virtual void render();
			virtual void renderChildren();

			typedef void (*luaRegisterFunc)(lua_State* L);
			static void registerLuaClass(std::string className, luaRegisterFunc register_metamethods, luaRegisterFunc register_methods, luaRegisterFunc register_getters, luaRegisterFunc register_setters, luaRegisterFunc register_events);

			virtual std::string toString();
			virtual void setParent(Instance* parent);

			virtual int wrap_lua(lua_State* L) = 0;

			static Instance* checkInstance(lua_State*, int index);

			static int lua_index(lua_State* L);
			static int lua_newindex(lua_State* L);

			static int lua_eq(lua_State* L);

			static int lua_toString(lua_State* L);

			static int lua_getClassName(lua_State* L);
			static int lua_getName(lua_State* L);
			static int lua_setName(lua_State* L);
			static int lua_getParent(lua_State* L);
			static int lua_setParent(lua_State* L);
			static int lua_getArchivable(lua_State* L);
			static int lua_setArchivable(lua_State* L);

			static int lua_readOnlyProperty(lua_State* L);

			static int lua_ClearAllChildren(lua_State* L);
			static int lua_Clone(lua_State* L);
			static int lua_Destroy(lua_State* L);
			static int lua_Remove(lua_State* L);
			static int lua_FindFirstChild(lua_State* L);
			static int lua_GetChildren(lua_State* L);
			static int lua_GetFullName(lua_State* L);
			static int lua_IsA(lua_State* L);
			static int lua_IsAncestorOf(lua_State* L);
			static int lua_IsDescendantOf(lua_State* L);

			DECLARE_STATIC_INIT(Instance);
		protected:
			bool Archivable;
			static std::string ClassName;
			static std::string LuaClassName;
			std::string Name;
			Instance* Parent;
			bool ParentLocked;

			virtual Instance* cloneImpl() = 0;

			virtual void removeChild(Instance* kid);
			virtual void addChild(Instance* kid);

			static void register_lua_metamethods(lua_State* L);
			static void register_lua_methods(lua_State* L);
			static void register_lua_property_setters(lua_State* L);
			static void register_lua_property_getters(lua_State* L);
			static void register_lua_events(lua_State* L);

			std::vector<Instance*> children;
	};
}

#endif
