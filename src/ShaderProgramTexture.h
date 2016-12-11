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

#include "ShaderProgramBaseMVPMatrix.h"
#include "UniformVariable.h"

class ShaderProgramTexture
{
private:
	struct ShaderVertPathTexture
	{
		static std::string getPath(){return "texture.vert";}
	};
	struct ShaderFragPathTexture
	{
		static std::string getPath(){return "texture.frag";}
	};
	
	using baseType = ShaderProgramBaseMVPMatrix<ShaderVertPathTexture,ShaderFragPathTexture>;
	baseType base;
	
	UniformVariable<int> textureSampler;
public:
	ShaderProgramTexture()
		:
			base(),
			textureSampler()
	{
	}
	void init(void)
	{
		base.init();
		
		Use<baseType> u(base);
		
		textureSampler.setLocation(base.getHandle(), "tex");
		textureSampler.set(0);
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
};
