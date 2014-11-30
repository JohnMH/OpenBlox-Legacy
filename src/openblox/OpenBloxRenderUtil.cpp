#include "OpenBloxRenderUtil.h"

#ifndef OPENBLOX_JNI
	void getFramebufferSize(int* width, int* height){
		glfwGetFramebufferSize(window, width, height);
	}
#else

#endif
