#pragma once
#include <iostream>
#include <GL\glew.h>
#include "GLTexture.h"

namespace SerraEngine {
	class Sprite
	{
		float _x;
		float _y;
		float _w;
		float _h;
		GLuint _vboID;
		GLTexture _texture;
	public:
		explicit Sprite();
		Sprite(float x, float y, float w, float h, const std::string &texturePath);
		~Sprite();

		void init(float x, float y, float w, float h, const std::string &texturePath);
		void draw() const;

		float getX() const { return _x; };
		float getY() const { return _y; };
		float getWidth() const { return _w; };
		float getHeight() const { return _h; };
	};
}
