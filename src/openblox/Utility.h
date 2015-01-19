#ifndef OPENBLOX_UTILITY_H_
#define OPENBLOX_UTILITY_H_

#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cstring>

#include <sys/time.h>
#include <unistd.h>

#include <string>

namespace OpenBlox{
	bool startsWith(std::string str, std::string str2);

	char* convertToLower(char* str);
	char* convertToLower(const char* str);

	char* convertToUpper(char* str);
	char* convertToUpper(const char* str);

	typedef int64_t msec_t;

	msec_t currentTimeMillis();

	char* fileGetContents(char* fileName);
	bool fileExists(char* fileName);
	bool fileIsReadable(char* fileName);
}
#endif
