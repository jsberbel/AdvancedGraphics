#include "GameEngine.h"
#include "ImageLoader.h"

GameEngine::GameEngine(const std::string &engineName, const int &screenWidth, const int &screenHeight) : 
	_window(new GLWindow(screenWidth, screenHeight, engineName)),
	_gameState(GameState::INIT),
	_screenWidth(screenWidth),
	_screenHeight(screenHeight),
	_time(0){
}

GameEngine::~GameEngine() {
	delete _window;
}

void GameEngine::initSystems() {
	SDL_Init(SDL_INIT_EVERYTHING);
	_window->createWindow(FULL_SCREEN);
}

void GameEngine::initShaders() {
	_program.compileShaders("./Shaders/colorShading.vert", "./Shaders/colorShading.frag");
	_program.addAttribute("vertexPosition");
	_program.addAttribute("vertexColor");
	_program.addAttribute("vertexUV");
	_program.linkShaders();
}


void pause_thread(int n)
{
	std::this_thread::sleep_for(std::chrono::seconds(n));
	std::cout << "pause of " << n << " seconds ended\n";
}
void GameEngine::gameLoop() {
	_sprite.init(-0.5f, -0.5f, 1, 1);
	_texture = ImageLoader::loadPNG("./Textures/serraplo.png");
	while (_gameState != GameState::EXIT) {
		processInput();
		_time += (float)0.001;
		drawGame();
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
	glBindTexture(GL_TEXTURE_2D, _texture.id);
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
	gameLoop();	
}
