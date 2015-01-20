#include "AssetLocator.h"

#include "OpenBlox.h"

#include <curl/curl.h>

#include <boost/filesystem.hpp>

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <stdio.h>

namespace OpenBlox {
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

	int calcDecodeLength(const char* b64input){
		int len = strlen(b64input);
		int padding = 0;

		if(b64input[len-1] == '=' && b64input[len-2] == '='){
			padding = 2;
		}else if (b64input[len-1] == '='){
			padding = 1;
		}

		return (int)len*0.75 - padding;
	}

	std::string stripContentType(std::string origin){
		std::vector<std::string> knownMimeTypes = {"text/plain", "text/html", "image/png", "image/jpeg"};

		for(std::vector<std::string>::size_type i = 0; i < knownMimeTypes.size(); i++){
			std::string knownType = knownMimeTypes[i];
			if(startsWith(origin, knownType)){
				origin = origin.substr(knownType.length());
				break;
			}
		}

		if(origin[0] == ';'){
			origin = origin.substr(1);
		}

		return origin;
	}

	asset_response_body* AssetLocator::getAsset(std::string url){
		if(hasAsset(url)){
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

		if(startsWith(url, "data:")){
			std::string nURL = url.substr(5);

			nURL = stripContentType(nURL);

			bool isBase64 = false;
			if(startsWith(nURL, "base64,")){
				nURL = nURL.substr(7);
				isBase64 = true;
			}else{
				if(startsWith(nURL, ",")){
					nURL = nURL.substr(1);
				}
			}

			if(nURL.length() == 0){
				return NULL;
			}

			if(isBase64){
				int decodeLen = calcDecodeLength(nURL.c_str()), len = 0;

				BIO* bio, *b64;
				char* buffer = new char[decodeLen + 1];

				b64 = BIO_new(BIO_f_base64());

				bio = BIO_new_mem_buf((void*)nURL.c_str(), nURL.length());
				bio = BIO_push(b64, bio);
				BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
				len = BIO_read(bio, (void*)buffer, nURL.length());
				buffer[len] = '\0';

				BIO_free_all(bio);

				body->data = buffer;
				body->size = len;

				return body;
			}

			char* data = new char[nURL.length() + 1];
			strcat(data, nURL.c_str());
			data[nURL.length() + 1] = '\0';

			body->data = data;
			body->size = strlen(data);

			return body;
		}

		if(startsWith(url, "res://")){
			boost::filesystem::path startPath = boost::filesystem::canonical(boost::filesystem::path("res/"));
			if(!boost::filesystem::exists(startPath)){
				boost::filesystem::create_directory(startPath);
			}

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

	void AssetLocator::putAsset(std::string url, std::string content){
		asset_response_body* body = new asset_response_body();
		body->data = "";
		body->size = 0;

		if(startsWith(content, "data:")){
			std::string nURL = content.substr(5);

			nURL = stripContentType(nURL);

			bool isBase64 = false;
			if(startsWith(nURL, "base64,")){
				nURL = nURL.substr(7);
				isBase64 = true;
			}else{
				if(startsWith(nURL, ",")){
					nURL = nURL.substr(1);
				}
			}
			if(nURL.length() == 0){
				body->data = "";
				body->size = 0;

				contentCache[url] = body;

				return;
			}

			if(isBase64){
				int decodeLen = calcDecodeLength(nURL.c_str()), len = 0;

				BIO* bio, *b64;
				char* buffer = new char[decodeLen + 1];

				b64 = BIO_new(BIO_f_base64());

				bio = BIO_new_mem_buf((void*)nURL.c_str(), nURL.length());
				bio = BIO_push(b64, bio);
				BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
				len = BIO_read(bio, (void*)buffer, nURL.length());
				buffer[len] = '\0';

				BIO_free_all(bio);

				body->data = buffer;
				body->size = len;

				contentCache[url] = body;

				return;
			}

			char* data = new char[nURL.length()];
			strcat(data, nURL.c_str());
			data[nURL.length()] = '\0';

			body->data = data;
			body->size = strlen(data);

			contentCache[url] = body;

			return;
		}

		char* new_data = new char[content.length()];
		strcat(new_data, content.c_str());
		new_data[content.length()] = '\0';

		body->data = new_data;
		body->size = strlen(new_data);

		contentCache[url] = body;
	}

	bool AssetLocator::hasAsset(std::string url){
		return contentCache.count(url) != 0;
	}
}
