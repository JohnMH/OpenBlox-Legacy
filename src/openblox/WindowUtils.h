#ifndef OPENBLOX_WINDOWUTILS_H_
#define OPENBLOX_WINDOWUTILS_H_

#ifndef _glfw3_h_
	#include <GLFW/glfw3.h>
#endif

#if defined(_WIN32)
	#define GLFW_EXPOSE_NATIVE_WIN32
	#define GLFW_EXPOSE_NATIVE_WGL
#elif defined(__unix__) || defined(__linux__)
	#define GLFW_EXPOSE_NATIVE_X11
	#define GLFW_EXPOSE_NATIVE_GLX
#elif defined(__APPLE__)
	#define GLFW_EXPOSE_NATIVE_COCOA
	#define GLFW_EXPOSE_NATIVE_NSGL
#else
	#error "Could not detect native environment."
#endif

#include <GLFW/glfw3native.h>

void glfwSetWindowMinimumSize(GLFWwindow* win, int minX, int minY);
void glfwMaximizeWindow(GLFWwindow* win);

#endif
