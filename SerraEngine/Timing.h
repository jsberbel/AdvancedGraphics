#pragma once
#include <SDL\SDL.h>

#define NUM_SAMPLES 10

namespace SerraEngine {

class FPSLimiter
{
	float _fps;
	float _targetFPS;
	Uint32 _startTicks;
	Uint32 _frameTicks;

	void calculateFPS();
public:
	FPSLimiter();
	FPSLimiter(float targetFPS);
	~FPSLimiter() = default;

	inline void setTargetFPS(float targetFPS) { _targetFPS = targetFPS; };
	inline float getFPS() const { return _fps; };

	inline void begin() { _startTicks = SDL_GetTicks(); };
	void end();
};

}

