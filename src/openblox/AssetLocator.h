#ifndef OPENBLOX_ASSETLOCATOR_H_
#define OPENBLOX_ASSETLOCATOR_H_

#include <string>

namespace OpenBlox{
	struct asset_response_body{
		size_t size;
		char* data;
	};
	class AssetLocator{
		public:
			static asset_response_body* getAsset(std::string url);
			static bool hasAsset(std::string url);
	};
}
#endif
