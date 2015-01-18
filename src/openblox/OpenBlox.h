#ifndef OPENBLOX_OPENBLOX_H_
#define OPENBLOX_OPENBLOX_H_

#include "OpenBloxConfig.h"

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include <cstdlib>
#include <cstring>
#include <cstdarg>

extern "C"{
	#include "../lua/lua.h"
	#include "../lua/lualib.h"
	#include "../lua/lauxlib.h"
}

#ifndef OPENBLOX_ANDROID

#ifndef OPENBLOX_SERVER
	#include <GL/glew.h>

	#define GLFW_DLL
	#include <GLFW/glfw3.h>
#endif

#define LOGI(...) printf(__VA_ARGS__); printf("\n"); fflush(stdout);
#define LOGW(...) printf(__VA_ARGS__); printf("\n"); fflush(stdout);
#define LOGE(...) fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);
#else
#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define LOG_TAG "libOpenBlox"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

#include "InternalException.h"

//Helpers
#include "static_init.h"
#include "Utility.h"
#include "Factory.h"
#include "Thread.h"
#include "ThreadScheduler.h"
#include "AssetLocator.h"

#include "BaseGame.h"
#endif
