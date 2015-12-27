#pragma once
#include <string>
#include <iostream>
#include <cstdlib>
#include <SDL\SDL.h>

class ErrorManager
{
public:
	static void errorRunTime(std::string errorString) {
		std::cout << errorString << std::endl;
		system("pause");
		SDL_Quit();
		exit(69);
	};
};


