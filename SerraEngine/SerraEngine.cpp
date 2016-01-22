#include "SerraEngine.h"
#include <SDL/SDL.h>

namespace SerraEngine {

	int init() {
		SDL_Init(SDL_INIT_EVERYTHING);
		//tell we want a double buffered windows to avoid flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		return 0;
	}

}
