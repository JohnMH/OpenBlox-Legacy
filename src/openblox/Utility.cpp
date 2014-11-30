#include "Utility.h"

namespace OpenBlox{
	char* covertToLower(char* ostr){
		int len = strlen(ostr);
		char* newstr = new char[len];
		for(int i = 0; i < len; i++){
			if(ostr[i] == '\0'){
				newstr[i] = '\0';
				break;
			}
			newstr[i] = tolower(ostr[i]);
		}
		return newstr;
	}

	char* covertToLower(const char* ostr){
		int len = strlen(ostr);
		char* newstr = new char[len];
		for(int i = 0; i < len; i++){
			if(ostr[i] == '\0'){
				newstr[i] = '\0';
				break;
			}
			newstr[i] = tolower(ostr[i]);
		}
		return newstr;
	}

	char* covertToUpper(char* ostr){
		int len = strlen(ostr);
		char* newstr = new char[len];
		for(int i = 0; i < len; i++){
			if(ostr[i] == '\0'){
				newstr[i] = '\0';
				break;
			}
			newstr[i] = toupper(ostr[i]);
		}
		return newstr;
	}

	char* covertToUpper(const char* ostr){
		int len = strlen(ostr);
		char* newstr = new char[len];
		for(int i = 0; i < len; i++){
			if(ostr[i] == '\0'){
				newstr[i] = '\0';
				break;
			}
			newstr[i] = toupper(ostr[i]);
		}
		return newstr;
	}
}
