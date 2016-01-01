#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace SerraEngine {

class Camera2D 
{
	int _screenWidth, _screenHeight;
	bool _needsMatrixUpdate;
	float _scale;
	glm::vec2 _position;
	glm::mat4 _cameraMatrix;
	glm::mat4 _orthoMatrix;
public:
	Camera2D();
	Camera2D(int screenWidth, int screenHeight);
	~Camera2D() = default;

	void init(int screenWidth, int screenHeight);
	void update();
	glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);

	//setters
	inline void setPosition(const glm::vec2 &newPos) { _position = newPos; _needsMatrixUpdate = true; };
	inline void setScale(float newScale) { _scale = newScale; _needsMatrixUpdate = true; };

	//getters
	inline glm::vec2 getPosition() const { return _position; };
	inline float getScale() const { return _scale; };
	inline glm::mat4 getCameraMatrix() const { return _cameraMatrix; };
};

}

