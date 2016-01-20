#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Vertex.h"
#include <vector>

namespace SerraEngine {

enum class GlyphSortType {
	NONE,
	FRONT_TO_BACK,
	BACK_TO_FRONT,
	TEXTURE
};

class Glyph {
public:
	GLuint texture;
	float depth;
	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;

	explicit Glyph() = default;
	explicit Glyph(GLuint texture_, float depth_, const glm::vec4 & destRect, const glm::vec4 & uvRect, const ColorRGBA8 &color) :
		texture(texture_),
		depth(depth_) 
	{
		topLeft.setColor(color);
		topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.setColor(color);
		bottomLeft.setPosition(destRect.x, destRect.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		bottomRight.setColor(color);
		bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.setColor(color);
		topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	};
};

struct RenderBatch {
	GLuint offset;
	GLuint numVertices;
	GLuint texture;
	explicit RenderBatch() {};
	explicit RenderBatch(GLuint offset_, GLuint numVertices_, GLuint texture_) :
		offset(offset_),
		numVertices(numVertices_),
		texture(texture_) {};
};

class SpriteBatch
{
	GLuint _vbo;
	GLuint _vao;
	GlyphSortType _sortType;

	std::vector<Glyph*> _glyphPointers; // Reference glyphs for sorting
	std::vector<Glyph> _glyphs; // Real used glyphs
	std::vector<RenderBatch> _renderBatches;

	void createRenderBatches();
	void createVertexArray();
	void sortGlyphs();

	static bool compareFrontToBack(Glyph* a, Glyph* b);
	static bool compareBackToFront(Glyph* a, Glyph* b);
	static bool compareTexture(Glyph* a, Glyph* b);
public:
	explicit SpriteBatch();
	explicit SpriteBatch(const SpriteBatch& spriteBatch);
	~SpriteBatch();

	void init();
	void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
	void end();
	void pushBatch(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const ColorRGBA8 & color);
	void renderBatch();
};

}

