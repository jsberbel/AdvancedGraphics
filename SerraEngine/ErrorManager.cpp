#include "ErrorManager.h"
#include <SDL\SDL.h>

namespace SerraEngine {
	void errorRunTime(const std::string &errorString) {
		SDL_Quit();
		throw std::exception(errorString.c_str());
	}
}