#pragma once
#include <string>
#include <iostream>
#include <SDL\SDL.h>

namespace SerraEngine {
	class ErrorManager {
	public:
		static void errorRunTime(std::string errorString) {
			std::cout << errorString << std::endl;
			system("pause");
			SDL_Quit();
			exit(69);
		};
	};
}


