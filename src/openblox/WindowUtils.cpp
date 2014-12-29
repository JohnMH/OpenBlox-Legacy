#include "WindowUtils.h"

#ifdef OPENBLOX_ANDROID

#elif defined(_WIN32)
	void glfwMaximizeWindow(GLFWwindow* window){
		if(window == NULL){
			return;
		}
		HWND nativeWin = glfwGetWin32Window(window);
		ShowWindow(nativeWin, SW_MAXIMIZE);
	}

	void glfwFocusWindow(GLFWwindow* window){
		if(window == NULL){
			return;
		}
		HWND nativeWin = glfwGetWin32Window(window);
		SetActiveWindow(nativeWin);
	}
#elif defined(__unix__) || defined(__linux__)
	void glfwMaximizeWindow(GLFWwindow* win){
		if(win == NULL){
			return;
		}
		Window nativeWin = glfwGetX11Window(win);
		Display* disp = glfwGetX11Display();

		XEvent xev;
		Atom wm_state = XInternAtom(disp, "_NET_WM_STATE", False);
		Atom max_horz = XInternAtom(disp, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
		Atom max_vert = XInternAtom(disp, "_NET_WM_STATE_MAXIMIZED_VERT", False);

		memset(&xev, 0, sizeof(xev));
		xev.type = ClientMessage;
		xev.xclient.window = nativeWin;
		xev.xclient.message_type = wm_state;
		xev.xclient.format = 32;
		xev.xclient.data.l[0] = _NET_WM_STATE_ADD;
		xev.xclient.data.l[1] = max_horz;
		xev.xclient.data.l[2] = max_vert;

		XSendEvent(disp, DefaultRootWindow(disp), False, SubstructureNotifyMask, &xev);
	}

	void glfwWindowMinSize(GLFWwindow* win, int width, int height){
		if(win == NULL){
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
		std::cout << "OpenBlox has not implemented glfwFocusWindow for this platform." << std::endl;
	}
#elif defined(__APPLE__)
	void glfwWindowMinSize(GLFWwindow* win, int width, int height){
		std::cout << "OpenBlox has not implemented glfwWindowMinSize for this platform." << std::endl;
	}

	void glfwMaximizeWindow(GLFWwindow* window){
		std::cout << "OpenBlox has not implemented glfwMaximizeWindow for this platform." << std::endl;
	}

	void glfwFocusWindow(GLFWwindow* win){
		std::cout << "OpenBlox has not implemented glfwFocusWindow for this platform." << std::endl;
	}
#endif
