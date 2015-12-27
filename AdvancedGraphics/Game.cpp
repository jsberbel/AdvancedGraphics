#include "Game.h"
#include <SerraEngine/ResourceManager.h>

Game::Game(const std::string &engineName, const int &screenWidth, const int &screenHeight) : 
	_window(new SerraEngine::GLWindow(screenWidth, screenHeight, engineName)),
	_camera(screenWidth, screenHeight),
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
	SerraEngine::init();
	_window->createWindow(); //Create the OpenGL window
	initShaders();
	_spriteBatch.init();
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
		_camera.update();
		drawGame();
		_fpsLimiter.calculateFPS();

		_fpsLimiter.setFrameTicks();
	}
}

void Game::processInput() {
	SDL_Event _event;

	const float CAMERA_SPEED = 10.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&_event)) {
		switch (_event.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			//std::cout << _event.motion.xrel << std::endl;
			break;
		case SDL_KEYDOWN:
			switch (_event.key.keysym.sym) {
			case SDLK_w:
				_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
				break;
			case SDLK_s:
				_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
				break;
			case SDLK_a:
				_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
				break;
			case SDLK_d:
				_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
				break;
			case SDLK_q:
				_camera.setScale(_camera.getScale() + SCALE_SPEED);
				break;
			case SDLK_e:
				_camera.setScale(_camera.getScale() - SCALE_SPEED);
				break;
			}
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

	//Set camera matrix
	GLint pLocation = _program.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.begin();

	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static SerraEngine::GLTexture tex = SerraEngine::ResourceManager::getTexture("./Textures/serraplo.png");
	_spriteBatch.draw(pos, uv, tex.id, 0.0f, SerraEngine::Color(255, 255, 255, 255));
	//_spriteBatch.draw(pos + glm::vec4(50.0f, 0.0f, 0.0f, 0.0f) , uv, tex.id, 0.0f, SerraEngine::Color(255, 255, 255, 255));

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	_program.unbind();

	_window->swapBuffer();
}

void Game::run() {
	initSystems();
	//_sprite.init(0.0f, 0.0f, _screenWidth/2, _screenHeight / 2, "./Textures/serraplo.png");
	gameLoop();	
}
