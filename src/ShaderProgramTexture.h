/*
 * =====================================================================================
 *
 *       Filename:  ShaderProgramTexture.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月08日 02時59分45秒
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
#include "VAOPositionTexture.h"

class ShaderProgramTexture
{
private:
	ShaderProgramVertexFragment shaderProgram;
	UniformVariable<glm::mat4> mvpMatrix;
	UniformVariable<int> textureSampler;
public:
	using vaoType = VAOPositionTexture<ShaderProgramTexture>;
	ShaderProgramTexture()
		:
			shaderProgram("texture.vert", "texture.frag"),
			mvpMatrix(),
			textureSampler()
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
};
