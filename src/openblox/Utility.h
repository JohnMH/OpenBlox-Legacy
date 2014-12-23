#ifndef OPENBLOX_UTILITY_H_
#define OPENBLOX_UTILITY_H_

#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cstring>

#include <sys/time.h>

namespace OpenBlox{
	char* covertToLower(char* str);
	char* covertToLower(const char* str);

	char* covertToUpper(char* str);
	char* covertToUpper(const char* str);

	long currentTimeMillis();

	char* fileGetContents(char* fileName);
}
#endif
