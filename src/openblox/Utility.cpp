#include "Utility.h"

namespace OpenBlox{
	char* covertToLower(char* s){
		if(s != NULL){
			int i;
			int sz = strlen(s);
			for(i = 0; i < sz; i++){
				s[i] = tolower(s[i]);
			}
		}
		return s;
	}

	char* covertToLower(const char* s){
		if(s != NULL){
			int i;
			int sz = strlen(s);
			char* newGuy = new char[sz];
			for(i = 0; i < sz; i++){
				newGuy[i] = tolower(s[i]);
			}
			return newGuy;
		}
		return NULL;
	}

	char* covertToUpper(char* s){
		if(s != NULL){
			int i;
			int sz = strlen(s);
			for(i = 0; i < sz; i++){
				s[i] = toupper(s[i]);
			}
		}
		return s;
	}

	char* covertToUpper(const char* s){
		if(s != NULL){
			int i;
			int sz = strlen(s);
			char* newGuy = new char[sz];
			for(i = 0; i < sz; i++){
				newGuy[i] = toupper(s[i]);
			}
			return newGuy;
		}
		return NULL;
	}

	long currentTimeMillis(){
		struct timeval tp;
		gettimeofday(&tp, NULL);
		long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
		return ms;
	}
}
