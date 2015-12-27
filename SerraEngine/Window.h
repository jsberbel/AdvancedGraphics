#pragma once
#include <iostream>
#include <SDL\SDL.h>
#include <GL\glew.h>
#include "ErrorManager.h"

class Window {
protected:
	SDL_Window* _SDLWindow;
	const std::string _engineName;
	int _screenWidth;
	int _screenHeight;
public:
	Window(const int &sw = 600, const int &sh = 600, const std::string &name = "") : _SDLWindow(nullptr), _screenWidth(sw), _screenHeight(sh), _engineName(name) {};
	~Window() {
		if (_SDLWindow != 0) SDL_DestroyWindow(_SDLWindow);
		SDL_Quit();
	};
	virtual void createWindow(bool fullScreen = false, const Uint32 &flag = 0) {
		_SDLWindow = SDL_CreateWindow(_engineName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, flag);
		if (_SDLWindow == nullptr) ErrorManager::errorRunTime("SDL Window could not be created.");
	};
	inline void changeSize(const int &sw, const int &sh) {
		SDL_SetWindowSize(_SDLWindow, sw, sh);
	};
	inline SDL_Window* getWindow() {
		return _SDLWindow;
	};
};

class GLWindow : public Window {
	SDL_GLContext glContext;
public:
	GLWindow(const int &sw = 600, const int &sh = 600, const std::string &name = "") : Window(sw, sh, name) {};
	~GLWindow() {
		if (_SDLWindow != 0) SDL_DestroyWindow(_SDLWindow);
		if (glContext != 0) SDL_GL_DeleteContext(glContext);
		SDL_Quit();
	};
	void createWindow(bool fullScreen = false, const Uint32 &flag = 0) {
		//tell we want a double buffered windows to avoid flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		_SDLWindow = SDL_CreateWindow(_engineName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL | flag | ((fullScreen) ? (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS):0));
		if (_SDLWindow == nullptr) ErrorManager::errorRunTime("SDL Window could not be created.");
		glContext = SDL_GL_CreateContext(_SDLWindow);
		if (glContext == nullptr) ErrorManager::errorRunTime("SDL_GL Context could not be created.");
		GLenum error = glewInit();
		if (error != GLEW_OK) ErrorManager::errorRunTime("GLEW could not be initialized.");
		std::printf("***  OpenGL Version: %s  ***\n", glGetString(GL_VERSION));
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	};
};