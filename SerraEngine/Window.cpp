#include "Window.h"
#include "ErrorManager.h"

namespace SerraEngine {

	Window::Window(int sw, int sh, const std::string &name) :
		_SDLWindow(nullptr),
		_screenWidth(sw),
		_screenHeight(sh),
		_engineName(name)
	{};

	Window::~Window() {
		if (_SDLWindow != 0) SDL_DestroyWindow(_SDLWindow);
		SDL_Quit();
	};

	void Window::createWindow(const unsigned &curFlags) {
		Uint32 flags = 0;
		if (curFlags & INVISIBLE) flags |= SDL_WINDOW_HIDDEN;
		else if (curFlags & FULLSCREEN) flags |= SDL_WINDOW_HIDDEN;
		else if (curFlags & BORDERLESS) flags |= SDL_WINDOW_HIDDEN;
		_SDLWindow = SDL_CreateWindow(_engineName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, flags);
		if (_SDLWindow == nullptr) ErrorManager::errorRunTime("SDL Window could not be created.");
	};


	GLWindow::GLWindow(int sw, int sh, const std::string &name) :
		Window(sw, sh, name)
	{};

	GLWindow::~GLWindow() {
		if (_SDLWindow != 0) SDL_DestroyWindow(_SDLWindow);
		if (glContext != 0) SDL_GL_DeleteContext(glContext);
		SDL_Quit();
	};

	void GLWindow::createWindow(const unsigned &curFlags) {
		Uint32 flags = SDL_WINDOW_OPENGL;
		if (curFlags & INVISIBLE) flags |= SDL_WINDOW_HIDDEN;
		else if (curFlags & FULLSCREEN) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		else if (curFlags & BORDERLESS) flags |= SDL_WINDOW_BORDERLESS;
		_SDLWindow = SDL_CreateWindow(_engineName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, flags);
		if (_SDLWindow == nullptr) ErrorManager::errorRunTime("SDL Window could not be created.");
		glContext = SDL_GL_CreateContext(_SDLWindow);
		if (glContext == nullptr) ErrorManager::errorRunTime("SDL_GL Context could not be created.");
		GLenum error = glewInit();
		if (error != GLEW_OK) ErrorManager::errorRunTime("GLEW could not be initialized.");
		std::printf("***  OpenGL Version: %s  ***\n", glGetString(GL_VERSION));
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	};

}