#include "Game.h"
#include <SerraEngine/ResourceManager.h>

Game::Game(const std::string &engineName, const int &screenWidth, const int &screenHeight) : 
	_window(new SerraEngine::GLWindow(screenWidth, screenHeight, engineName)),
	_camera(screenWidth, screenHeight),
	_gameState(GameState::INIT),
	_screenWidth(screenWidth),
	_screenHeight(screenHeight),
	_time(0.0f),
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
		_fpsLimiter.begin();

		processInput();
		_time += 0.01f;
		_camera.update();
		//Update bullets
		for (int i = 0; i < _bullets.size(); i++) {
			if (_bullets[i].update()) _bullets[i] = _bullets.back(), _bullets.pop_back();
		}
		drawGame();

		_fpsLimiter.end();
	}
}

void Game::processInput() {
	SDL_Event _event;

	const float CAMERA_SPEED = 5.0f;
	const float SCALE_SPEED = 0.05f;

	while (SDL_PollEvent(&_event)) {
		switch (_event.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(_event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(_event.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(_event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(_event.button.button);
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords((float)_event.motion.x, (float)_event.motion.y);
			break;
		}
	}

	if (_inputManager.isKeyPressed(SDLK_w)) _camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	if (_inputManager.isKeyPressed(SDLK_s)) _camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	if (_inputManager.isKeyPressed(SDLK_a)) _camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	if (_inputManager.isKeyPressed(SDLK_d)) _camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	if (_inputManager.isKeyPressed(SDLK_q)) _camera.setScale(_camera.getScale() + SCALE_SPEED);
	if (_inputManager.isKeyPressed(SDLK_e)) _camera.setScale(_camera.getScale() - SCALE_SPEED);
	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		glm::vec2 playerPosition(10.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);
		_bullets.emplace_back(playerPosition, direction, 5.0f, 1000);
	}
}

void Game::drawGame() {
	glClearDepth(1.0); //Set the base depth to 1.0
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the color and depth buffer

	_program.bind(); //Enable the shader

	glActiveTexture(GL_TEXTURE0); //Use texture unit 0
	//glBindTexture(GL_TEXTURE_2D, _texture.id);
	GLint textureLocation = _program.getUniformLocation("sampler"); //Get the uniform location
	glUniform1i(textureLocation, 0); //Tell the shader that the texture is in texture unit 0

	//GLuint timeLocation = _program.getUniformLocation("time");
	//glUniform1f(timeLocation, _time); //Set the constantly changing time variable

	GLint pLocation = _program.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0])); //Set camera matrix

	_spriteBatch.begin();

		glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
		glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
		static SerraEngine::GLTexture tex = SerraEngine::ResourceManager::getTexture("./Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
		_spriteBatch.draw(pos, uv, tex.id, 0.0f, SerraEngine::Color(255, 255, 255, 255));
		
		for (Bullet b : _bullets) b.draw(_spriteBatch);

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0); //unbind the texture

	_program.unbind(); //disable the shader

	_window->swapBuffer(); //Swap our buffer and draw everything to the screen
}

void Game::run() {
	initSystems();
	//_sprite.init(0.0f, 0.0f, _screenWidth/2, _screenHeight / 2, "./Textures/serraplo.png");
	gameLoop();	
}
