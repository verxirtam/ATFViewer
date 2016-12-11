/*
 * =====================================================================================
 *
 *       Filename:  ShaderProgramString.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年12月11日 02時56分22秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include "ShaderProgramBaseMVPMatrix.h"
#include "UniformVariable.h"

class ShaderProgramString
{
private:
	struct ShaderVertPathTexture
	{
		static std::string getPath(){return "string.vert";}
	};
	struct ShaderFragPathTexture
	{
		static std::string getPath(){return "string.frag";}
	};
	ShaderProgramBaseMVPMatrix<ShaderVertPathTexture,ShaderFragPathTexture> base;
	UniformVariable<int> textureSampler;
	UniformVariable<glm::vec3> color;

public:
	ShaderProgramString()
		:
			base(),
			textureSampler(),
			color()
	{
	}
	void init(void)
	{
		base.init();
		textureSampler.setLocation(base.getHandle(), "tex");
		textureSampler.set(0);
		color.setLocation(base.getHandle(), "stringColor");
		color.set(glm::vec3(1.0f, 0.0f, 0.0f));
	}
	void use()
	{
		base.use();
	}
	void unuse()
	{
		base.unuse();
	}
	void setMVPMatrix(const glm::mat4& m)
	{
		base.setMVPMatrix(m);
	}
	void setColor(const glm::vec3& c)
	{
		color.set(c);
	}

};
