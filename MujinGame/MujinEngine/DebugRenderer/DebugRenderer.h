#pragma once

#include "../GLSLProgram.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../Vertex.h"
#include "../Camera2.5D/CameraManager.h"
#include <vector>


class DebugRenderer {
public:
	DebugRenderer();
	~DebugRenderer();

	void init();
	void end();
	void drawBox(const glm::vec4& destRect, const Color& color, float angle);
	void drawCircle(const glm::vec2& center, const Color& color, float radius);
	void render(const glm::mat4& projectionMatrix, float lineWidth);
	void dispose();

	struct DebugVertex { //instead of using the general Vertex that has also info about texture
							// we use this where we want just color
		glm::vec2 position;
		Color color;
	};

private:
	GLSLProgram _program;
	std::vector<DebugVertex> _verts;
	std::vector<GLuint> _indices;
	GLuint _vbo = 0, _vao = 0, _ibo = 0; //! ibo is what is going to store the integers for each 
	//! vertex so we can use index drawing, without specifying the lines by duplicating the vertices
	int _numElements = 0;
};