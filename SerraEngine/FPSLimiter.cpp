#include "FPSLimiter.h"

FPSLimiter::FPSLimiter() :_fps(0), _maxFPS(120.0f), _startTicks(0), _frameTicks(0)
{
}

FPSLimiter::FPSLimiter(float maxFPS) : _fps(0), _maxFPS(maxFPS), _startTicks(0), _frameTicks(0)
{
}

FPSLimiter::~FPSLimiter()
{
}

void FPSLimiter::calculateFPS()
{
	static Uint32 frameTimes[NUM_SAMPLES];
	static int curFrame = 0;

	static auto prevTicks = SDL_GetTicks();
	auto curTicks = SDL_GetTicks();

	frameTimes[curFrame] = curTicks - prevTicks;
	prevTicks = curTicks;

	static int count = 0;
	curFrame++;
	if (count < NUM_SAMPLES) count = curFrame;
	else count = NUM_SAMPLES;
	if (curFrame == NUM_SAMPLES) curFrame = 0;

	Uint32 frameTimeAverage = 0;
	for (int i = 0; i < count; i++) frameTimeAverage += frameTimes[i];
	frameTimeAverage /= count;

	if (frameTimeAverage > 0) _fps = 1000.0f / frameTimeAverage;
	else _fps = 60.0f;
}

void FPSLimiter::setFrameTicks() {
	_frameTicks = SDL_GetTicks() - _startTicks;
	if (1000.0f / _maxFPS > _frameTicks) SDL_Delay((Uint32) (1000.0f / _maxFPS - _frameTicks));
};
