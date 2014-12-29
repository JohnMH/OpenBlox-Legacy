#ifndef OPENBLOX_OPENBLOXRENDERUTIL_H_
#define OPENBLOX_OPENBLOXRENDERUTIL_H_

#include "OpenBlox.h"

namespace OpenBlox{
	extern bool renderLock;

	void getFramebufferSize(int* width, int* height);
	bool createGLContext();
	#ifndef OPENBLOX_ANDROID
	GLFWwindow* getWindow();
	#endif
}

#endif
