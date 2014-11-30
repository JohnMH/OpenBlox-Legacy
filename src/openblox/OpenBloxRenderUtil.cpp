#include "OpenBloxRenderUtil.h"

#ifndef OPENBLOX_JNI
#include "WindowUtils.h"
#endif

namespace OpenBlox{
#ifndef OPENBLOX_JNI
	static GLFWwindow* window;

	void getFramebufferSize(int* width, int* height){
		if(window == NULL){
			std::cerr << "Window is NULL!" << std::endl;
			return;
		}
		glfwGetFramebufferSize(window, width, height);
	}

	bool createGLContext(){
		window = glfwCreateWindow(640, 480, "OpenBlox", NULL, NULL);
		if(!window){
			std::cerr << "Could not create window." << std::endl;
			glfwTerminate();
			return false;
		}
		glfwMaximizeWindow(window);
		#if defined(__unix__) || defined(__linux__)
		glfwWindowMinSize(window, 640, 480);
		#endif
		return true;
	}

	GLFWwindow* getWindow(){
		return window;
	}
#else
	void getFramebufferSize(int* width, int* height){
		int w = 1;
		int h = 1;

		width = &w;
		height = &h;
	}

	bool createGLContext(){
		return false;
	}
#endif
}
