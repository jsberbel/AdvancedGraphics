#include "Game.h"

Game::Game(const std::string &engineName, const int &screenWidth, const int &screenHeight) : 
	_window(new GLWindow(screenWidth, screenHeight, engineName)),
	_gameState(GameState::INIT),
	_screenWidth(screenWidth),
	_screenHeight(screenHeight),
	_time(0),
	_fpsLimiter(120.0f){
}

Game::~Game() {
	delete _window;
}

void Game::initSystems() {
	SDL_Init(SDL_INIT_EVERYTHING);
	_window->createWindow(FULL_SCREEN);
	//set vsync
	SDL_GL_SetSwapInterval(0);
}

void Game::initShaders() {
	_program.compileShaders("./Shaders/colorShading.vert", "./Shaders/colorShading.frag");
	_program.addAttribute("vertexPosition");
	_program.addAttribute("vertexColor");
	_program.addAttribute("vertexUV");
	_program.linkShaders();
}

void Game::gameLoop() {
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

void Game::processInput() {
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

void Game::drawGame() {
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

void Game::run() {
	initSystems();
	initShaders();
	_sprite.init(-0.5f, -0.5f, 1, 1, "./Textures/serraplo.png");
	gameLoop();	
}
