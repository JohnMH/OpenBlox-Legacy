#ifndef OB_INSTANCE_HTTPSERVICE_H_
#define OB_INSTANCE_HTTPSERVICE_H_

#include "Instance.h"

#include "../ob_type/WebSocket.h"

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

			std::string GetAsync(const char* url, bool nocache = false);
			//std::string PostAsync(char* url, char* data, Enum HttpContentType content_type = ApplicationJson); //GOD DAMN IT DEVDIGGY! DO ENUMS!

			std::string UrlEncode(std::string input);
			std::string UrlDecode(std::string input);

			ob_type::WebSocket* CreateWebSocket(const char* uri);

			std::string GenerateGUID(bool wrapInCurlyBraces = true);

			virtual Instance* cloneImpl();

			virtual std::string getClassName();

			virtual int wrap_lua(lua_State* L);

			DECLARE_STATIC_INIT(HttpService);
		protected:
			static std::string ClassName;
			static std::string LuaClassName;

			static void register_lua_methods(lua_State* L);
	};
}
#endif
