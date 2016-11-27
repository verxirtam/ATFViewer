/*
 * =====================================================================================
 *
 *       Filename:  ShaderProgramPositionColor.h
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
#include "ShaderProgramVertexFragment.h"
#include "UniformVariable.h"
#include "VAOPositionColor.h"


class ShaderProgramPositionColor
{
private:
	ShaderProgramVertexFragment shaderProgram;
	UniformVariable<glm::mat4> mvpMatrix;
public:
	ShaderProgramPositionColor()
		:
			shaderProgram("basic.vert", "basic.frag"),
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
	using vaoType = VAOPositionColor<ShaderProgramPositionColor>;
	using vaoTypeDynamic = VAOPositionColorDynamic<ShaderProgramPositionColor>;
};

