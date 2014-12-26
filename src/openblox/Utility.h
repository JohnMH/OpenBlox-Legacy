#ifndef OPENBLOX_UTILITY_H_
#define OPENBLOX_UTILITY_H_

#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cstring>

#include <sys/time.h>
#include <unistd.h>

namespace OpenBlox{
	char* covertToLower(char* str);
	char* covertToLower(const char* str);

	char* covertToUpper(char* str);
	char* covertToUpper(const char* str);

	typedef int64_t msec_t;

	msec_t currentTimeMillis();

	char* fileGetContents(char* fileName);
	bool fileExists(char* fileName);
	bool fileIsReadable(char* fileName);
}
#endif
