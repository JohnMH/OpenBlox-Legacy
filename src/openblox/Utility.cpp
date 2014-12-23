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

	char* fileGetContents(char* fileName){
		char* file_contents;
		long file_size;

		FILE *input_file = fopen(fileName, "rb");
		fseek(input_file, 0, SEEK_END);
		file_size = ftell(input_file);
		rewind(input_file);
		file_contents = (char*)malloc((file_size + 1) * sizeof(char));
		fread(file_contents, sizeof(char), file_size, input_file);
		fclose(input_file);

		file_contents[file_size] = '\0';

		return file_contents;
	}
}
