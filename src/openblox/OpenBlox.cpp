#include "OpenBlox.h"

#include "../ob_instance/DataModel.h"

#include <unistd.h>
#include <stdio.h>

#include "WindowUtils.h"

#include "OpenBloxRenderUtil.h"

OpenBlox::BaseGame* game;

double lastTime = glfwGetTime();
int nbFrames = 0;

void render(){
	double currentTime = glfwGetTime();
	nbFrames++;
	if(currentTime - lastTime >= 1.0){
		std::string newTitle = "OpenBlox - TPS: ";
		newTitle = newTitle + ((std::ostringstream&)(std::ostringstream() << std::dec << 1000.0/double(nbFrames))).str();

		glfwSetWindowTitle(OpenBlox::getWindow(), newTitle.c_str());

		nbFrames = 0;
		lastTime += 1.0;
	}

	float ratio;
	int width, height;

	OpenBlox::getFramebufferSize(&width, &height);
	ratio = width / (float)height;

	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);

	ob_instance::DataModel* dm = game->getDataModel();
	if(dm != NULL){
		dm->render();
		dm->renderChildren();
	}
}

void* luaThread(void* arg){
	lua_State* L = OpenBlox::BaseGame::newLuaState();
	lua_resume(L, 0);

	char* script = "while true do print('Hi');delay(5, function() print('lolo'); end);print(wait(1));print('Bye');spawn(function(...)print('waiting 1 sec')wait(1)print('waited')end) end";
	int s = luaL_loadbuffer(L, script, strlen(script), "@game.Workspace.Script");
	if(s == 0){
		s = lua_pcall(L, 0, LUA_MULTRET, 0);
	}

	if(s != 0){
		game->handle_lua_errors(L);
	}

	GLFWwindow* window = OpenBlox::getWindow();
	while(!glfwWindowShouldClose(window)){
		game->getThreadScheduler()->Tick();
	}
	lua_close(L);

	pthread_exit(NULL);
	return NULL;
}

void size_callback(int width, int height){
	//Update stuff
}

#ifndef OPENBLOX_JNI
void glfw_error_callback(int error, const char* description){
	LOGE("[GLFW] %s", description);
}

void glfw_window_size_callback(GLFWwindow* window, int width, int height){
	size_callback(width, height);
}

void* renderThread(void* arg){
	GLFWwindow* window = OpenBlox::getWindow();
	glfwMakeContextCurrent(window);

	while(!glfwWindowShouldClose(window)){
		//Fire RunService.Stepped, then RunService.RenderStepped
		render();

		glfwSwapBuffers(window);
	}
	pthread_exit(NULL);
	return NULL;
}

int main(){
	glfwSetErrorCallback(glfw_error_callback);
	if(!glfwInit()){
		LOGE("[GLFW] Failed to initialize library.");
		return 1;
	}

	OpenBlox::BaseGame::InstanceFactory = new OpenBlox::Factory();

	game = new OpenBlox::BaseGame();

	static_init::execute();

	#ifdef __APPLE__
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}
	#endif
	glfwWindowHint(GLFW_SAMPLES, OPENBLOX_AA_SAMPLES);

	if(!OpenBlox::createGLContext()){
		return 1;
	}

	GLFWwindow* window = OpenBlox::getWindow();

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		LOGE("[GLFW] Failed to initialize GLEW.");
		glfwTerminate();
		return 1;
	}

	glfwSetWindowSizeCallback(window, glfw_window_size_callback);

	{
		const GLubyte* vendor = glGetString(GL_VENDOR);
		const GLubyte* renderer = glGetString(GL_RENDERER);
		const GLubyte* version = glGetString(GL_VERSION);
		const GLubyte* shading_version = glGetString(GL_SHADING_LANGUAGE_VERSION);

		LOGI("[GL] Vendor: %s", vendor);
		LOGI("[GL] Renderer: %s", renderer);
		LOGI("[GL] OpenGL Version: %s", version);
		LOGI("[GL] Shading Language Version: %s", shading_version);
	}

	pthread_t render_thread;
	int val;
	val = pthread_create(&render_thread, NULL, renderThread, NULL);
	if(val){
		LOGE("[CORE] Failed to create render thread.");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(NULL);

	pthread_t lua_thread;
	val = pthread_create(&lua_thread, NULL, luaThread, NULL);
	if(val){
		LOGE("[CORE] Failed to create logic thread.");
		glfwTerminate();
		return 1;
	}

	while(!glfwWindowShouldClose(window)){
		glfwPollEvents();
	}

	void* status;
	pthread_join(lua_thread, &status);
	pthread_join(render_thread, &status);

	glfwDestroyWindow(window);
	OpenBlox::BaseGame::getInstanceFactory()->releaseTable();
	glfwTerminate();
	return 0;
}
#endif
