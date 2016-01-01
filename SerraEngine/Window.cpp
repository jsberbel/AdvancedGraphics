#include "Window.h"
#include "ErrorManager.h"

namespace SerraEngine {

	//========================================================
	//						WINDOW
	//========================================================

	Window::Window(int sw, int sh, const std::string &name) :
		_SDLWindow(nullptr),
		_engineName(name),
		_screenWidth(sw),
		_screenHeight(sh)
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
		if (_SDLWindow == nullptr) errorRunTime("SDL Window could not be created.");
	};

	//========================================================
	//						GLWINDOW
	//========================================================

	GLWindow::GLWindow(int sw, int sh, const std::string &name) :
		glContext(nullptr),
		Window(sw, sh, name)
	{};

	GLWindow::~GLWindow() {
		if (_SDLWindow != 0) SDL_DestroyWindow(_SDLWindow);
		if (glContext != 0) SDL_GL_DeleteContext(glContext);
		SDL_Quit();
	};

	void GLWindow::createWindow(const Color& c, const unsigned &curFlags) {
		Uint32 flags = SDL_WINDOW_OPENGL;
		if (curFlags & INVISIBLE) flags |= SDL_WINDOW_HIDDEN;
		else if (curFlags & FULLSCREEN) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		else if (curFlags & BORDERLESS) flags |= SDL_WINDOW_BORDERLESS;
		_SDLWindow = SDL_CreateWindow(_engineName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, flags);
		if (_SDLWindow == nullptr) errorRunTime("SDL Window could not be created.");
		glContext = SDL_GL_CreateContext(_SDLWindow);
		if (glContext == nullptr) errorRunTime("SDL_GL Context could not be created.");
		GLenum error = glewInit();
		if (error != GLEW_OK) errorRunTime("GLEW could not be initialized.");
		std::printf("***  OpenGL Version: %s  ***\n", glGetString(GL_VERSION));
		glClearColor((float)c.r/255, (float)c.g / 255, (float)c.b / 255, (float)c.a / 255); //Set background color
		SDL_GL_SetSwapInterval(0); //Set V-Sync
		//Enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	};

}