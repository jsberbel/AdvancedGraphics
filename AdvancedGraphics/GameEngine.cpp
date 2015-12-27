#include "GameEngine.h"

GameEngine::GameEngine(const std::string &engineName, const int &screenWidth, const int &screenHeight) : 
	_window(new GLWindow(screenWidth, screenHeight, engineName)),
	_gameState(GameState::INIT),
	_screenWidth(screenWidth),
	_screenHeight(screenHeight),
	_time(0),
	_fpsLimiter(120.0f){
}

GameEngine::~GameEngine() {
	delete _window;
}

void GameEngine::initSystems() {
	SDL_Init(SDL_INIT_EVERYTHING);
	_window->createWindow(FULL_SCREEN);
	//set vsync
	SDL_GL_SetSwapInterval(0);
}

void GameEngine::initShaders() {
	_program.compileShaders("./Shaders/colorShading.vert", "./Shaders/colorShading.frag");
	_program.addAttribute("vertexPosition");
	_program.addAttribute("vertexColor");
	_program.addAttribute("vertexUV");
	_program.linkShaders();
}

void GameEngine::gameLoop() {
	while (_gameState != GameState::EXIT) {
		_fpsLimiter.setStartTicks();

		processInput();
		_time += 0.01f;
		drawGame();
		_fpsLimiter.calculateFPS();

		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == NUM_SAMPLES) {
			std::cout << _fpsLimiter.getFPS() << std::endl;
			frameCounter = 0;
		}

		_fpsLimiter.setFrameTicks();
	}
}

void GameEngine::processInput() {
	SDL_Event _event;

	while (SDL_PollEvent(&_event)) {
		switch (_event.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			//std::cout << _event.motion.xrel << std::endl;
			break;
		}
	}
}

void GameEngine::drawGame() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_program.bind();

	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, _texture.id);
	GLint textureLocation = _program.getUniformLocation("sampler");
	glUniform1i(textureLocation, 0);

	GLuint timeLocation = _program.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	_sprite.draw();

	glBindTexture(GL_TEXTURE_2D, 0);

	_program.unbind();

	SDL_GL_SwapWindow(_window->getWindow());
}

void GameEngine::run() {
	initSystems();
	initShaders();
	_sprite.init(-0.5f, -0.5f, 1, 1, "./Textures/serraplo.png");
	gameLoop();	
}
