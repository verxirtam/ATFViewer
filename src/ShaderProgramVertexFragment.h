/*
 * =====================================================================================
 *
 *       Filename:  ShaderProgramVertexFragment.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年11月28日 00時15分12秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include "ShaderProgram.h"

class ShaderProgramVertexFragment
{
private:
	Shader vertShader;
	Shader fragShader;
	ShaderProgram shaderProgram;
public:
	ShaderProgramVertexFragment(const std::string& vertshaderfilename, const std::string& fragshaderfilename)
		:
			vertShader(GL_VERTEX_SHADER,  vertshaderfilename),
			fragShader(GL_FRAGMENT_SHADER, fragshaderfilename),
			shaderProgram()
	{
	}
	void init(void);
	void use()
	{
		shaderProgram.use();
	}
	void unuse()
	{
		shaderProgram.unuse();
	}
	GLuint getHandle()
	{
		return shaderProgram.getHandle();
	}
};
