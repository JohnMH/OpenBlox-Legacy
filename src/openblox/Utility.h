#ifndef OPENBLOX_UTILITY_H_
#define OPENBLOX_UTILITY_H_

#include "OpenBlox.h"

namespace OpenBlox{
	char *covertToLower(char *str);
	char *covertToLower(const char *str);

	char *covertToUpper(char *str);
	char *covertToUpper(const char *str);

	char *my_strdup(const char *str);
}

#endif
