#pragma once
#include <glm/glm.hpp>
#include<unordered_map>

namespace SerraEngine {

class InputManager
{
	glm::vec2 _mouseCoords;
	std::unordered_map<unsigned int, bool> _keyMap;
	std::unordered_map<unsigned int, bool> _prevKeyMap;
public:
	InputManager();
	~InputManager() = default;
	void update();

	void pressKey(unsigned keyID);
	void releaseKey(unsigned keyID);
	bool isKeyDown(unsigned keyID);
	bool isKeyDown(const std::unordered_map<unsigned int, bool> &keyMap, unsigned keyID);
	bool isKeyPressed(unsigned keyID);

	void setMouseCoords(float x, float y);

	inline glm::vec2 getMouseCoords() const { return _mouseCoords; };
};

}


