#include "ErrorManager.h"

namespace SerraEngine {
	void errorRunTime(const std::string &errorString) {
		std::cout << errorString << std::endl;
		system("pause");
		SDL_Quit();
		exit(69);
	}
}