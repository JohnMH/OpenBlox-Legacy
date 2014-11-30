#ifndef OPENBLOX_OPENBLOXRENDERUTIL_H_
#define OPENBLOX_OPENBLOXRENDERUTIL_H_

#include "OpenBlox.h"

namespace OpenBlox{
	void getFramebufferSize(int* width, int* height);
	bool createGLContext();
	#ifndef OPENBLOX_JNI
	GLFWwindow* getWindow();
	#endif
}

#endif
