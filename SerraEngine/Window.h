#pragma once
#include <SDL\SDL.h>
#include <string>
#include "Vertex.h"

namespace SerraEngine {
	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window {
	protected:
		SDL_Window* m_SDLWindow;
		const std::string m_engineName;
		int m_screenWidth;
		int m_screenHeight;
	public:
		explicit Window(int sw = 600, int sh = 600, const std::string &name = "");
		virtual ~Window();
		virtual void createWindow(const unsigned &curFlags = 0);
		void changeSize(const int &sw, const int &sh) const { SDL_SetWindowSize(m_SDLWindow, sw, sh); };
		SDL_Window* getWindow() const { return m_SDLWindow; };
		void swapBuffer() const { SDL_GL_SwapWindow(m_SDLWindow); };
	};

	class GLWindow : public Window {
		SDL_GLContext m_glContext;
	public:
		explicit GLWindow(int sw = 600, int sh = 600, const std::string &name = "");
		~GLWindow();
		void createWindow(const ColorRGBA8& c = ColorRGBA8{ 255, 255, 255, 255 }, const unsigned &curFlags = 0);
	};
}