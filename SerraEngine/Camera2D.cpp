#include "Camera2D.h"

namespace SerraEngine {

Camera2D::Camera2D() :
	_screenWidth(0),
	_screenHeight(0),
	_needsMatrixUpdate(true), 
	_scale(0.0f), 
	_position(0.0f, 0.0f),
	_cameraMatrix(1.0f),
	_orthoMatrix(1.0f)
{}

Camera2D::Camera2D(int screenWidth, int screenHeight) :
	_screenWidth(screenWidth),
	_screenHeight(screenHeight),
	_needsMatrixUpdate(true),
	_scale(1.0f),
	_position(0.0f, 0.0f),
	_cameraMatrix(1.0f),
	_orthoMatrix(1.0f)
{
	_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
}

void Camera2D::init(int screenWidth, int screenHeight) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
}

void Camera2D::update() {
	if (_needsMatrixUpdate) {
		_cameraMatrix = glm::translate(_orthoMatrix, glm::vec3(-_position.x+ _screenWidth / 2, -_position.y+ _screenHeight / 2, 0.0f));
		_cameraMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(_scale, _scale, 0.0f))*_cameraMatrix;
		_needsMatrixUpdate = false;
	}
}

glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) {
	screenCoords.y = _screenHeight - screenCoords.y; //invert y direction
	screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2); //make 0 center of screen
	screenCoords /= _scale; //scale coordinates
	screenCoords += _position; //add camera position
	return screenCoords;
}

bool Camera2D::isBoxInView(const glm::vec2 & position, const glm::vec2 & dimensions) {
	glm::vec2 scaledScreenDimensions = glm::vec2(_screenWidth, _screenHeight) / (_scale);

	const float MIN_DISTANCE_X = dimensions.x * 0.5f + scaledScreenDimensions.x * 0.5f;
	const float MIN_DISTANCE_Y = dimensions.y * 0.5f + scaledScreenDimensions.y * 0.5f;

	glm::vec2 centerPos = _position + (dimensions * 0.5f);
	glm::vec2 distVec = centerPos - position;

	float xDepth = MIN_DISTANCE_X - abs(distVec.x);
	float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

	if (xDepth > 0 && yDepth > 0) return true;
	return false;
}

}