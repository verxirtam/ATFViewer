#pragma once



#include "OpenGLHeaders.h"


class TestVBO
{
private:
	GLuint vbo;
	GLuint vao;
public:
	~TestVBO()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
	void init();
	void display();
};





