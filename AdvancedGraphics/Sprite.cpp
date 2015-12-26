#include "Sprite.h"
#include "ResourceManager.h"

Sprite::Sprite() : _vboID(0)
{
	
}

Sprite::Sprite(float x, float y, float w, float h, const std::string &texturePath) : _vboID(0)
{
	init(x, y, w, h, texturePath);
}

Sprite::~Sprite()
{
	if (_vboID != 0) glDeleteBuffers(1, &_vboID);
}

void Sprite::init(float x, float y, float w, float h, const std::string &texturePath) {
	_x = x;
	_y = y;
	_w = w;
	_h = h;
	_texture = ResourceManager::getTexture(texturePath);

	//generate buffer if it is empty
	if (_vboID == 0) glGenBuffers(1, &_vboID);

	Vertex vertexData[6];//1 quad = 2 triangles
	//triangle1
	vertexData[0].setPosition(x + w, y + h);
	vertexData[0].setUV(1.0f, 1.0f);
	vertexData[1].setPosition(x, y + h);
	vertexData[1].setUV(0.0f, 1.0f);
	vertexData[2].setPosition(x, y);
	vertexData[2].setUV(0.0f, 0.0f);
	//triangle2
	vertexData[3].setPosition(x, y);
	vertexData[3].setUV(0.0f, 0.0f);
	vertexData[4].setPosition(x + w, y);
	vertexData[4].setUV(1.0f, 0.0f);
	vertexData[5].setPosition(x + w, y + h);
	vertexData[5].setUV(1.0f, 1.0f);

	//for (int i = 0; i < 6; i++) vertexData[i].color = { 0,0,255,255 };
	vertexData[0].color = vertexData[2].color = vertexData[3].color = vertexData[5].color = { 0,0,255,255 };
	vertexData[1].color = vertexData[4].color = { 255,0,0,255 };

	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::draw() {
	glBindTexture(GL_TEXTURE_2D, _texture.id);
	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	//use first attribute array
	glEnableVertexAttribArray(0);
	//position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	//color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	//uv attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	//draw vertices on screen
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//disable first attribute array
	glDisableVertexAttribArray(0);
	//unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}