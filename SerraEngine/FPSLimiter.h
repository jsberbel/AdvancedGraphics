#pragma once
#include <SDL\SDL.h>

#define NUM_SAMPLES 10

class FPSLimiter
{
	float _fps;
	float _maxFPS;
	Uint32 _startTicks;
	Uint32 _frameTicks;
public:
	FPSLimiter();
	FPSLimiter(float maxFPS);
	~FPSLimiter();
	inline float getFPS() const { return _fps; };
	inline void setStartTicks() { _startTicks = SDL_GetTicks(); };
	void setFrameTicks();
	void calculateFPS();
};

