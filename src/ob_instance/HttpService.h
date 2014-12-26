#ifndef OB_INSTANCE_HTTPSERVICE_H_
#define OB_INSTANCE_HTTPSERVICE_H_

#include "Instance.h"

namespace ob_instance{
	class HttpService: public Instance{
		public:
			struct response_body{
				size_t size;
				char* data;
			};

			HttpService();
			virtual ~HttpService();

			static size_t write_data(void* ptr, size_t size, size_t nmemb, struct response_body* data);

			char* GetAsync(const char* url, bool nocache = false);
			//char* PostAsync(char* url, char* data, Enum HttpContentType content_type = ApplicationJson); //GOD DAMN IT DEVDIGGY! DO ENUMS!

			char* UrlEncode(const char* input);

			virtual Instance* cloneImpl();

			virtual char* getClassName();

			virtual int wrap_lua(lua_State* L);

			DECLARE_STATIC_INIT(HttpService);
		protected:
			static char* ClassName;
			static char* LuaClassName;

			static void register_lua_methods(lua_State* L);
	};
}
#endif
