#include "InputManager.h"

namespace SerraEngine {

InputManager::InputManager() : _mouseCoords(0.0f,0.0f)
{}

void InputManager::update() {
	for (auto &it : _keyMap) {
		_prevKeyMap[it.first] = it.second;
	}
}

void InputManager::pressKey(unsigned keyID) {
	_keyMap[keyID] = true;
}

void InputManager::releaseKey(unsigned keyID) {
	_keyMap[keyID] = false;
}

bool InputManager::isKeyDown(unsigned keyID) {
	auto it = _keyMap.find(keyID);
	if (it != _keyMap.end()) return it->second; //key found
	return false;
}

bool InputManager::isKeyDown(const std::unordered_map<unsigned int, bool> &keyMap, unsigned keyID) {
	auto it = keyMap.find(keyID);
	if (it != keyMap.end()) return it->second; //key found
	return false;
}

bool InputManager::isKeyPressed(unsigned keyID) {
	return (isKeyDown(keyID) && !isKeyDown(_prevKeyMap, keyID));
}

void InputManager::setMouseCoords(float x, float y) {
	_mouseCoords.x = x;
	_mouseCoords.y = y;
}

}
