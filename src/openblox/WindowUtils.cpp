#include "WindowUtils.h"

#if defined(_WIN32)
	void glfwSetWindowMinimumSize(GLFWwindow* window, int minX, int minY){
		if(window == NULL){
			return;
		}
		HWND nativeWin = glfwGetWin32Window(window);
	}

	void glfwMaximizeWindow(GLFWwindow* window){
		if(window == NULL){
			return;
		}
		HWND nativeWin = glfwGetWin32Window(window);
		ShowWindow(nativeWin, SW_MAXIMIZE);
	}
#elif defined(__unix__) || defined(__linux__)
	void glfwSetWindowMinimumSize(GLFWwindow* window, int minX, int minY){
		std::cout << "OpenBlox has not implemented glfwSetWindowMinimumSize for this platform." << std::endl;
	}

	void glfwMaximizeWindow(GLFWwindow* window){
		std::cout << "OpenBlox has not implemented glfwMaximizeWindow for this platform." << std::endl;
	}
#elif defined(__APPLE__)
	void glfwSetWindowMinimumSize(GLFWwindow* window, int minX, int minY){
		std::cout << "OpenBlox has not implemented glfwSetWindowMinimumSize for this platform." << std::endl;
	}

	void glfwMaximizeWindow(GLFWwindow* window){
		std::cout << "OpenBlox has not implemented glfwMaximizeWindow for this platform." << std::endl;
	}
#endif
