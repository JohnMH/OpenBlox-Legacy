#include "AssetLocator.h"

#include "OpenBlox.h"

#include <curl/curl.h>

#include <boost/filesystem.hpp>

namespace OpenBlox{
	static std::map<std::string, asset_response_body*> contentCache = std::map<std::string, asset_response_body*>();

	size_t write_data(void* ptr, size_t size, size_t nmemb, struct asset_response_body* data){
		size_t index = data->size;
		size_t n = (size * nmemb);
		char* tmp;

		data->size += n;

		tmp = (char*)realloc(data->data, data->size + 1);

		if(tmp){
			data->data = tmp;
		}else{
			if(data->data){
				delete[] data->data;
			}
			LOGE("[AssetLocator] Failed to allocate memory.");
			return 0;
		}

		memcpy((data->data + index), ptr, n);
		data->data[data->size] = '\0';

		return n;
	}

	asset_response_body* AssetLocator::getAsset(std::string url){
		if(contentCache.count(url)){
			return contentCache[url];
		}

		if(startsWith(url, "file://")){
			return NULL;
		}

		asset_response_body* body = new asset_response_body();
		body->size = 0;
		body->data = new char[4096];
		if(body->data == NULL){
			LOGE("[AssetLocator] Failed to allocate memory.");
			return NULL;
		}

		body->data[0] = '\0';

		if(startsWith(url, "res://")){
			boost::filesystem::path startPath = boost::filesystem::canonical(boost::filesystem::path("res/"));
			std::string startStr = startPath.generic_string();

			std::string toAdd = url.substr(6);
			boost::filesystem::path newPath = boost::filesystem::path(startStr + "/" + toAdd);
			if(boost::filesystem::exists(newPath)){
				newPath = boost::filesystem::canonical(newPath);

				const char* newPathStr = newPath.generic_string().c_str();
				if(startsWith(newPathStr, startStr.c_str())){
					if(boost::filesystem::is_regular_file(newPath)){
						char* file_contents;
						long file_size;

						FILE *input_file = fopen(newPathStr, "rb");
						fseek(input_file, 0, SEEK_END);
						file_size = ftell(input_file);
						rewind(input_file);
						file_contents = (char*)malloc((file_size + 1) * sizeof(char));
						fread(file_contents, sizeof(char), file_size, input_file);
						fclose(input_file);

						//file_contents[file_size] = '\0';

						body->data = file_contents;
						body->size = file_size;

						contentCache[url] = body;

						return body;
					}
				}
			}

			return NULL;
		}

		CURL* curl;
		CURLcode res;

		curl = curl_easy_init();
		if(curl){
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);

			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, body);

			res = curl_easy_perform(curl);
			if(res != CURLE_OK){
				LOGE("[AssetLocator] cURL Error: %s", curl_easy_strerror(res));
			}

			curl_easy_cleanup(curl);

			contentCache[url] = body;

			return body;
		}

		return NULL;
	}

	bool AssetLocator::hasAsset(std::string url){
		std::string lstring = std::string(convertToLower(url.c_str()));

		return contentCache.count(lstring) != 0;
	}
}
