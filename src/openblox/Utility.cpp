#include "Utility.h"

namespace OpenBlox{
	char* covertToLower(char* str){
		char* newstr, *p;
		p = newstr = my_strdup(str);
		while(*p++=tolower(*p));

		return newstr;
	}

	char* covertToLower(const char* str){
		char* newstr, *p;
		p = newstr = my_strdup(str);
		while(*p++=tolower(*p));

		return newstr;
	}

	char* covertToUpper(char* str){
		char* newstr, *p;
		p = newstr = my_strdup(str);
		while(*p++=toupper(*p));

		return newstr;
	}
	char* covertToUpper(const char* str){
		char *newstr, *p;
		p = newstr = my_strdup(str);
		while(*p++=toupper(*p));

		return newstr;
	}

	char* my_strdup(const char* str){
	    size_t len = strlen(str);
	    char *x = (char*)malloc(len+1);
	    if(!x){
	    	return NULL;
	    }
	    memcpy(x, str, len+1);
	    return x;
	}
}
