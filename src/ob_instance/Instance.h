#ifndef OB_INSTANCE_INSTANCE_H_
#define OB_INSTANCE_INSTANCE_H_

#include "../openblox/OpenBlox.h"

namespace ob_instance{
	class Instance{
		public:
			Instance();
			virtual ~Instance();

			virtual void ClearAllChildren();
			virtual Instance* Clone();
			virtual void Destroy();
			virtual void Remove();
			virtual Instance* FindFirstChild(char* name, bool recursive = false);
			virtual Instance** GetChildren();
			virtual char* GetFullName();
			virtual bool IsA(const char *name);
			virtual bool IsAncestorOf(Instance* descendant);
			virtual bool IsDescendantOf(Instance* ancestor);
			//virtual Instance* WaitForChild(char* childName);

			virtual char* toString();
			virtual void setParent(Instance* parent);

			virtual int wrap_lua(lua_State* L) = 0;

			static int lua_toString(lua_State* L);

			static int lua_ClearAllChildren(lua_State* L);
			static int lua_Clone(lua_State* L);
			static int lua_Destroy(lua_State* L);
			static int lua_Remove(lua_State* L);
			static int lua_FindFirstChild(lua_State* L);
			static int lua_GetChildren(lua_State* L);
			static int lua_GetFullName(lua_State* L);
			static int lua_IsA(lua_State* L);
			static int lua_IsAncestorOf(lua_State* L);
			static int lua_IsDescendant(lua_State* L);

			DECLARE_STATIC_INIT(Instance);
		protected:
			bool Archivable;
			static char* ClassName;
			static char* LuaClassName;
			char* Name;
			Instance* Parent;

			virtual Instance* cloneImpl() = 0;

			virtual void removeChild(Instance* kid);
			virtual void addChild(Instance* kid);

			static void register_lua_methods(lua_State* L);

			std::vector<Instance*> children;
	};
}

#endif
