#include "AssetLocator.h"

#include <map>

#include "Utility.h"

namespace OpenBlox{
	static std::map<std::string, char*> contentCache = std::map<std::string, char*>();

	char* AssetLocator::getAsset(std::string url){
		std::string lstring = std::string(convertToLower(url.c_str()));

		if(contentCache.count(lstring)){
			return contentCache[lstring];
		}

		return NULL;
	}

	bool AssetLocator::hasAsset(std::string url){
		std::string lstring = std::string(convertToLower(url.c_str()));

		return contentCache.count(lstring) != 0;
	}
}
