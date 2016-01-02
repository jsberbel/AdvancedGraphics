#include "Human.h"

Human::Human(const glm::vec2 & position, float speed, const SerraEngine::Color& color) :
	Agent(position, speed, color) {}

Human::~Human()
{
}
