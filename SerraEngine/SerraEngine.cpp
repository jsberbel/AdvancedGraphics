#include "SerraEngine.h"

namespace SerraEngine {

	int init() {
		SDL_Init(SDL_INIT_EVERYTHING);
		//tell we want a double buffered windows to avoid flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		//Set V-Sync
		SDL_GL_SetSwapInterval(0);
		return 0;
	}

}
