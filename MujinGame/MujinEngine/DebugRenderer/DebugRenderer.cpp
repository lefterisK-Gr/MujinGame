#include "DebugRenderer.h"

DebugRenderer::DebugRenderer()
{
}

DebugRenderer::~DebugRenderer()
{
	dispose();
}

void DebugRenderer::init()
{
	// Shader init


	//Set up buffers
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ibo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo); //Buffer used for index drawing

	// todo: vertex attrib pointers

	glBindVertexArray(0); //! the buffers are bound withing the context of vao so when that is
								//! unbinded all the other vbos are also unbinded
}

void DebugRenderer::end()
{
}

void DebugRenderer::drawBox(const glm::vec4& destRect, const Color& color, float angle)
{
}

void DebugRenderer::drawCircle(const glm::vec2& center, const Color& color, float radius)
{
}

void DebugRenderer::render()
{
}

void DebugRenderer::dispose()
{
}
