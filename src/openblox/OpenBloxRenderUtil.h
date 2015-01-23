#ifndef OPENBLOX_OPENBLOXRENDERUTIL_H_
#define OPENBLOX_OPENBLOXRENDERUTIL_H_

#include "OpenBlox.h"

namespace OpenBlox{
	extern bool renderLock;
	extern int calculatedFPS;

	void getFramebufferSize(int* width, int* height);
	bool createGLContext();
	#if !defined(OPENBLOX_ANDROID) && !defined(OPENBLOX_SERVER)
	GLFWwindow* getWindow();
	#endif
}

#endif
