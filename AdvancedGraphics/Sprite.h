#pragma once
#include <iostream>
#include <GL\glew.h>
#include "Vertex.h"

class Sprite
{
	float _x;
	float _y;
	float _w;
	float _h;
	GLuint _vboID;
public:
	Sprite();
	~Sprite();

	void init(float x, float y, float w, float h);
	void draw();
};

