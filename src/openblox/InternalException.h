#ifndef OPENBLOX_INTERNALEXCEPTION_H_
#define OPENBLOX_INTERNALEXCEPTION_H_

#include <string>
#include <stdexcept>

namespace OpenBlox{
	struct InternalException: public std::exception{
		const char* s;
		InternalException(const char* ss){
			s = ss;
		}
		~InternalException() throw(){}
		const char* what() const throw(){return s;}
	};
}
#endif
