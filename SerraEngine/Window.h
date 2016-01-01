#pragma once
#include <SDL\SDL.h>
#include <GL\glew.h>
#include <string>
#include "Vertex.h"

namespace SerraEngine {
	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window {
	protected:
		SDL_Window* _SDLWindow;
		const std::string _engineName;
		int _screenWidth;
		int _screenHeight;
	public:
		Window(int sw = 600, int sh = 600, const std::string &name = "");
		~Window();
		virtual void createWindow(const unsigned &curFlags = 0);
		inline void changeSize(const int &sw, const int &sh) const { SDL_SetWindowSize(_SDLWindow, sw, sh); };
		inline SDL_Window* getWindow() const { return _SDLWindow; };
		inline void swapBuffer() const { SDL_GL_SwapWindow(_SDLWindow); };
	};

	class GLWindow : public Window {
		SDL_GLContext glContext;
	public:
		GLWindow(int sw = 600, int sh = 600, const std::string &name = "");
		~GLWindow();
		void createWindow(const Color& c = { 255, 255, 255, 255 }, const unsigned &curFlags = 0);
	};
}