#ifndef OPENBLOX_ASSETLOCATOR_H_
#define OPENBLOX_ASSETLOCATOR_H_

#include <string>

namespace OpenBlox{
	class AssetLocator{
		public:
			static char* getAsset(std::string url);
			static bool hasAsset(std::string url);
	};
}
#endif
