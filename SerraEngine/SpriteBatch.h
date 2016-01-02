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

struct Glyph {
	GLuint texture;
	float depth;
	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;
	Glyph() {};
	Glyph(GLuint texture_, float depth_) :
		texture(texture_),
		depth(depth_) {};
};

struct RenderBatch {
	GLuint offset;
	GLuint numVertices;
	GLuint texture;
	RenderBatch() {};
	RenderBatch(GLuint offset_, GLuint numVertices_, GLuint texture_) :
		offset(offset_),
		numVertices(numVertices_),
		texture(texture_) {};
};

class SpriteBatch
{
	GLuint _vbo;
	GLuint _vao;
	GlyphSortType _sortType;

	std::vector<Glyph* > _glyphs;
	std::vector<RenderBatch> _renderBatches;

	void createRenderBatches();
	void createVertexArray();
	void sortGlyphs();

	static bool compareFrontToBack(Glyph* a, Glyph* b);
	static bool compareBackToFront(Glyph* a, Glyph* b);
	static bool compareTexture(Glyph* a, Glyph* b);
public:
	SpriteBatch();
	SpriteBatch(const SpriteBatch& spriteBatch);
	~SpriteBatch() = default;

	void init();
	void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
	void end();
	void pushBatch(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const Color & color);
	void renderBatch();
};

}

