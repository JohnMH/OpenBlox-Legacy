#ifndef OPENBLOX_WINDOWUTILS_H_
#define OPENBLOX_WINDOWUTILS_H_

#ifndef OPENBLOX_JNI

#include <iostream>

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

void glfwMaximizeWindow(GLFWwindow* win);
void glfwRestoreWindow(GLFWwindow* win);
void glfwMinimizeWindow(GLFWwindow* win);
void glfwFocusWindow(GLFWwindow* win);

#if defined(__unix__) || defined(__linux__)
	void glfwWindowMinSize(GLFWwindow* win, int width, int height);
#endif

#endif
#endif
