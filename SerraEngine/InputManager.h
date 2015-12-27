#pragma once
#include <glm/glm.hpp>
#include<unordered_map>

namespace SerraEngine {

class InputManager
{
	glm::vec2 _mouseCoords;
	std::unordered_map<unsigned int, bool> _keyMap;
public:
	InputManager();
	~InputManager();

	void pressKey(unsigned keyID);
	void releaseKey(unsigned keyID);
	bool isKeyPressed(unsigned keyID);
	void setMouseCoords(float x, float y);

	inline glm::vec2 getMouseCoords() const { return _mouseCoords; };
};

}


