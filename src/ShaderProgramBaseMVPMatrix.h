/*
 * =====================================================================================
 *
 *       Filename:  ShaderProgramBaseMVPMatrix.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年11月28日 05時15分48秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include "ShaderProgramBase.h"
#include "UniformVariable.h"


template
<
	typename ShaderVertPathClass,
	typename ShaderFragPathClass
>
class ShaderProgramBaseMVPMatrix
{
private:
	using baseType = ShaderProgramBase<ShaderVertPathClass, ShaderFragPathClass>;
	baseType base;
	UniformVariable<glm::mat4> mvpMatrix;
public:
	ShaderProgramBaseMVPMatrix():
			base(),
			mvpMatrix()
	{
	}
	void init(void)
	{
		base.init();
		mvpMatrix.setLocation(base.getHandle(), "mvpMatrix");
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
		base.use();
		mvpMatrix.set(m);
		base.unuse();
	}
	GLuint getHandle()
	{
		return base.getHandle();
	}
};
