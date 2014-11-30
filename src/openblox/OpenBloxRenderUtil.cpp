#include "OpenBloxRenderUtil.h"

#ifndef OPENBLOX_JNI
#include "WindowUtils.h"
#endif

namespace OpenBlox{
#ifndef OPENBLOX_JNI
	static GLFWwindow* window;

	std::map<std::string, GLuint> shaderMap;
	std::map<std::string, GLuint> programMap;

	void getFramebufferSize(int* width, int* height){
		if(window == NULL){
			return;
		}
		glfwGetFramebufferSize(window, width, height);
	}

	bool createGLContext(){
		window = glfwCreateWindow(640, 480, "OpenBlox", NULL, NULL);
		if(!window){
			LOGE("[GLFW] Could not create window.");
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
