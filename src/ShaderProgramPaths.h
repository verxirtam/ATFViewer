/*
 * =====================================================================================
 *
 *       Filename:  ShaderProgramPaths.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月31日 19時44分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include "Shader.h"
#include "ShaderProgram.h"
#include "UniformVariable.h"
#include "VAOPaths.h"


class ShaderProgramPaths
{
private:
	Shader vertShader;
	Shader fragShader;
	ShaderProgram shaderProgram;
	UniformVariable<glm::mat4> mvpMatrix;
public:
	ShaderProgramPaths()
		:
			vertShader(GL_VERTEX_SHADER,  "paths.vert"),
			fragShader(GL_FRAGMENT_SHADER,"paths.frag"),
			shaderProgram(),
			mvpMatrix()
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
	void setMVPMatrix(const glm::mat4& m)
	{
		shaderProgram.use();
		mvpMatrix.set(m);
		shaderProgram.unuse();
	}
	using vaoTypeDynamic = VAOPaths<ShaderProgramPaths>;
};

