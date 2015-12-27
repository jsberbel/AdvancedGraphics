#pragma once
#include <iostream>
#include <GL\glew.h>
#include "GLTexture.h"
#include "Vertex.h"
#include <string>

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
		Sprite();
		Sprite(float x, float y, float w, float h, const std::string &texturePath);
		~Sprite();

		void init(float x, float y, float w, float h, const std::string &texturePath);
		void draw();

		inline float getX() const { return _x; };
		inline float getY() const { return _y; };
		inline float getWidth() const { return _w; };
		inline float getHeight() const { return _h; };
	};
}
