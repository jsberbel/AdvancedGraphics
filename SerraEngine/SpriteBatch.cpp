#include "SpriteBatch.h"
#include <algorithm>

namespace SerraEngine {
	
SpriteBatch::SpriteBatch() : _vbo(0), _vao(0), _sortType(GlyphSortType::NONE)
{}

SpriteBatch::SpriteBatch(const SpriteBatch& spriteBatch) {
	*this = spriteBatch;
}

void SpriteBatch::init() {
	createVertexArray();
}

void SpriteBatch::begin(GlyphSortType sortType){
	_sortType = sortType;
	_renderBatches.clear();
	for (auto g : _glyphs) delete g, g = nullptr;
	_glyphs.clear();
}

void SpriteBatch::end(){
	sortGlyphs();
	createRenderBatches();
}

void SpriteBatch::pushBatch(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const Color &color) {
	Glyph* temp = new Glyph(texture, depth);

	temp->topLeft.setColor(color);
	temp->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
	temp->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

	temp->bottomLeft.setColor(color);
	temp->bottomLeft.setPosition(destRect.x, destRect.y);
	temp->bottomLeft.setUV(uvRect.x, uvRect.y);

	temp->bottomRight.setColor(color);
	temp->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
	temp->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

	temp->topRight.setColor(color);
	temp->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
	temp->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

	_glyphs.push_back(temp);
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
	if (_glyphs.empty()) return;

	std::vector<Vertex> vertices;
	vertices.resize(_glyphs.size() * 6); //6 vertices for each glyph

	int offset = 0; //current offset
	int curVertex = 0; //current vertex
	//Add the first batch
	_renderBatches.emplace_back(offset, 6, _glyphs[0]->texture); //push_back that creates an object automatically with the parameters
	vertices[curVertex++] = _glyphs[0]->topLeft;
	vertices[curVertex++] = _glyphs[0]->bottomLeft;
	vertices[curVertex++] = _glyphs[0]->bottomRight;
	vertices[curVertex++] = _glyphs[0]->bottomRight;
	vertices[curVertex++] = _glyphs[0]->topRight;
	vertices[curVertex++] = _glyphs[0]->topLeft;
	offset += 6;
	for (unsigned curGlyph = 1; curGlyph < _glyphs.size(); curGlyph++) {
		if (_glyphs[curGlyph]->texture != _glyphs[curGlyph - 1]->texture) {
			_renderBatches.emplace_back(offset, 6, _glyphs[curGlyph]->texture);
		}
		else _renderBatches.back().numVertices += 6;
		vertices[curVertex++] = _glyphs[curGlyph]->topLeft;
		vertices[curVertex++] = _glyphs[curGlyph]->bottomLeft;
		vertices[curVertex++] = _glyphs[curGlyph]->bottomRight;
		vertices[curVertex++] = _glyphs[curGlyph]->bottomRight;
		vertices[curVertex++] = _glyphs[curGlyph]->topRight;
		vertices[curVertex++] = _glyphs[curGlyph]->topLeft;
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
		std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack);
		break;
	case GlyphSortType::BACK_TO_FRONT:
		std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront);
		break;
	case GlyphSortType::TEXTURE:
		std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture);
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