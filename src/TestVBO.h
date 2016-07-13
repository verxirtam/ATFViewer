#pragma once



#include "OpenGLHeaders.h"


class TestVBO
{
private:
	GLuint vbo;
public:
	~TestVBO()
	{
		glDeleteBuffers(1, &vbo);
	}
	void init()
	{
		float cx = 139.0f;
		float cy = 35.0f;
		float cz = 30000.0f;
		float v[8][3]
			= {
				{cx - 1.0f, cy - 1.0f, cz},
				{cx - 1.0f, cy + 1.0f, cz},
				{cx + 1.0f, cy - 1.0f, cz},
				{cx + 1.0f, cy + 1.0f, cz},
				{cx - 1.0f, cy - 1.0f, cz + 20000.0f},
				{cx - 1.0f, cy + 1.0f, cz + 20000.0f},
				{cx + 1.0f, cy - 1.0f, cz + 20000.0f},
				{cx + 1.0f, cy + 1.0f, cz + 20000.0f}
			};
		
		
		
		
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		GLuint size = 8 * 3 * sizeof(float);
		glBufferData(GL_ARRAY_BUFFER, size, v, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void display();
};





