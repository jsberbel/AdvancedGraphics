#include "SpriteBatch.h"
#include <algorithm>

namespace SerraEngine {
	
SpriteBatch::SpriteBatch() : _vbo(0), _vao(0), _sortType(GlyphSortType::NONE)
{}

SpriteBatch::SpriteBatch(const SpriteBatch& spriteBatch) {
	*this = spriteBatch;
}

SpriteBatch::~SpriteBatch() {
	for (auto g : _glyphPointers) if(!g) delete g, g = nullptr;
}

void SpriteBatch::init() {
	createVertexArray();
}

void SpriteBatch::begin(GlyphSortType sortType){
	_sortType = sortType;
	_renderBatches.clear();
	_glyphs.clear();
}

void SpriteBatch::end(){
	_glyphPointers.resize(_glyphs.size());
	for (unsigned i = 0; i < _glyphs.size(); i++) _glyphPointers[i] = &_glyphs[i];
	sortGlyphs();
	createRenderBatches();
}

void SpriteBatch::pushBatch(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const ColorRGBA8 &color) {
	_glyphs.emplace_back(texture, depth, destRect, uvRect, color);
}

void SpriteBatch::renderBatch() {
	glBindVertexArray(_vao);
	for (RenderBatch rb : _renderBatches) {
		glBindTexture(GL_TEXTURE_2D, rb.texture);
		glDrawArrays(GL_TRIANGLES, rb.offset, rb.numVertices);
	}
	glBindVertexArray(0);
}

void SpriteBatch::createRenderBatches() {
	if (_glyphPointers.empty()) return;

	std::vector<Vertex> vertices;
	vertices.resize(_glyphPointers.size() * 6); //6 vertices for each glyph

	int offset = 0; //current offset
	int curVertex = 0; //current vertex
	//Add the first batch
	_renderBatches.emplace_back(offset, 6, _glyphPointers[0]->texture); //push_back that creates an object automatically with the parameters
	vertices[curVertex++] = _glyphPointers[0]->topLeft;
	vertices[curVertex++] = _glyphPointers[0]->bottomLeft;
	vertices[curVertex++] = _glyphPointers[0]->bottomRight;
	vertices[curVertex++] = _glyphPointers[0]->bottomRight;
	vertices[curVertex++] = _glyphPointers[0]->topRight;
	vertices[curVertex++] = _glyphPointers[0]->topLeft;
	offset += 6;
	for (unsigned curGlyph = 1; curGlyph < _glyphPointers.size(); curGlyph++) {
		if (_glyphPointers[curGlyph]->texture != _glyphPointers[curGlyph - 1]->texture) {
			_renderBatches.emplace_back(offset, 6, _glyphPointers[curGlyph]->texture);
		}
		else _renderBatches.back().numVertices += 6;
		vertices[curVertex++] = _glyphPointers[curGlyph]->topLeft;
		vertices[curVertex++] = _glyphPointers[curGlyph]->bottomLeft;
		vertices[curVertex++] = _glyphPointers[curGlyph]->bottomRight;
		vertices[curVertex++] = _glyphPointers[curGlyph]->bottomRight;
		vertices[curVertex++] = _glyphPointers[curGlyph]->topRight;
		vertices[curVertex++] = _glyphPointers[curGlyph]->topLeft;
		offset += 6;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	//orphan the buffer
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	//upload the data
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(Vertex), vertices.data());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteBatch::createVertexArray() {
	if (!_vao) glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	if (!_vbo) glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	//use attribute arrays
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	//color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	//uv attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0);
}

void SpriteBatch::sortGlyphs() {
	switch (_sortType)
	{
	case GlyphSortType::FRONT_TO_BACK:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
		break;
	case GlyphSortType::BACK_TO_FRONT:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
		break;
	case GlyphSortType::TEXTURE:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
		break;
	default:
		break;
	}
}

bool SpriteBatch::compareFrontToBack(Glyph * a, Glyph * b) {
	return (a->depth < b->depth);
}

bool SpriteBatch::compareBackToFront(Glyph * a, Glyph * b) {
	return (a->depth > b->depth);
}

bool SpriteBatch::compareTexture(Glyph * a, Glyph * b) {
	return (a->texture < b->texture);
}

}