#include "WindowUtils.h"

#if defined(OPENBLOX_JNI)

#elif defined(_WIN32)
	void glfwMaximizeWindow(GLFWwindow* window){
		if(window == NULL){
			return;
		}
		HWND nativeWin = glfwGetWin32Window(window);
		ShowWindow(nativeWin, SW_MAXIMIZE);
	}

	void glfwRestoreWindow(GLFWwindow* window){
		if(window == NULL){
			return;
		}
		HWND nativeWin = glfwGetWin32Window(window);
		ShowWindow(nativeWin, SW_RESTORE);
	}

	void glfwMinimizeWindow(GLFWwindow* window){
		if(window == NULL){
			return;
		}
		HWND nativeWin = glfwGetWin32Window(window);
		ShowWindow(nativeWin, SW_MINIMIZE);
	}

	void glfwFocusWindow(GLFWwindow* window){
		if(window == NULL){
			return;
		}
		HWND nativeWin = glfwGetWin32Window(window);
		SetActiveWindow(nativeWin);
	}
#elif defined(__unix__) || defined(__linux__)
	void glfwSetWindowMinimumSize(GLFWwindow* window, int minX, int minY){
		std::cout << "OpenBlox has not implemented glfwSetWindowMinimumSize for this platform." << std::endl;
	}

	void glfwMaximizeWindow(GLFWwindow* window){
		std::cout << "OpenBlox has not implemented glfwMaximizeWindow for this platform." << std::endl;
	}

	void glfwRestoreWindow(GLFWwindow* window){
		std::cout << "OpenBlox has not implemented glfwRestoreWindow for this platform." << std::endl;
	}

	void glfwMinimizeWindow(GLFWwindow* window){
		std::cout << "OpenBlox has not implemented glfwRestoreWindow for this platform." << std::endl;
	}

	void glfwWindowMinSize(GLFWwindow* win, int width, int height){
		if(window == NULL){
			return;
		}
		Window nativeWin = glfwGetX11Window(win);
		Display* disp = glfwGetX11Display();

		XSizeHints* hints = XAllocSizeHints();
		hints->flags |= PMinSize;
		hints->min_width = width;
		hints->min_height = height;

		XSetWMNormalHints(disp, nativeWin, hints);
		XFree(hints);

		XFlush(disp);
	}

	void glfwFocusWindow(GLFWwindow* win){
		std::cout << "OpenBlox has not implemented glfwSetWindowMinimumSize for this platform." << std::endl;
	}
#elif defined(__APPLE__)
	void glfwSetWindowMinimumSize(GLFWwindow* window, int minX, int minY){
		std::cout << "OpenBlox has not implemented glfwSetWindowMinimumSize for this platform." << std::endl;
	}

	void glfwMaximizeWindow(GLFWwindow* window){
		std::cout << "OpenBlox has not implemented glfwMaximizeWindow for this platform." << std::endl;
	}

	void glfwRestoreWindow(GLFWwindow* window){
		std::cout << "OpenBlox has not implemented glfwRestoreWindow for this platform." << std::endl;
	}

	void glfwMinimizeWindow(GLFWwindow* window){
		std::cout << "OpenBlox has not implemented glfwRestoreWindow for this platform." << std::endl;
	}

	void glfwFocusWindow(GLFWwindow* win){
		std::cout << "OpenBlox has not implemented glfwSetWindowMinimumSize for this platform." << std::endl;
	}
#endif
