#include "InputManager.h"

namespace SerraEngine {

InputManager::InputManager() : _mouseCoords(0.0f,0.0f)
{}


InputManager::~InputManager()
{}

void InputManager::pressKey(unsigned keyID) {
	_keyMap[keyID] = true;
}

void InputManager::releaseKey(unsigned keyID) {
	_keyMap[keyID] = false;
}

bool InputManager::isKeyPressed(unsigned keyID) {
	auto it = _keyMap.find(keyID);
	if (it != _keyMap.end()) return it->second; //key found
	return false;
}

void InputManager::setMouseCoords(float x, float y) {
	_mouseCoords.x = x;
	_mouseCoords.y = y;
}

}
