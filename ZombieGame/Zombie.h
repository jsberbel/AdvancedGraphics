#pragma once
#include "Agent.h"

class Zombie : public Agent
{
public:
	Zombie() = default;
	~Zombie();
	void update() {};
};

