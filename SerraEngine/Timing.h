#pragma once
#include <SDL\SDL.h>

namespace SerraEngine {

class FPSLimiter
{
	float _fps;
	float _targetFPS;
	Uint32 _startTicks;
	Uint32 _frameTicks;
	static const int NUM_SAMPLES;

	void calculateFPS();
public:
	FPSLimiter();
	FPSLimiter(float targetFPS);
	~FPSLimiter() = default;

	inline void setTargetFPS(float targetFPS) { _targetFPS = targetFPS; };
	void getFPS() const;

	inline void begin() { _startTicks = SDL_GetTicks(); };
	void end();
};

}

