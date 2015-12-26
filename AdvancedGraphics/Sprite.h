#pragma once
#include <iostream>
#include <GL\glew.h>
#include "GLTexture.h"
#include "Vertex.h"
#include <string>

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
};

