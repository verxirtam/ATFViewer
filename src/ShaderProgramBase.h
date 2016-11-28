/*
 * =====================================================================================
 *
 *       Filename:  ShaderProgramBase.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年11月28日 04時08分13秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <string>

#include "Shader.h"
#include "ShaderProgram.h"


template
<
	typename ShaderVertPathClass,
	typename ShaderFragPathClass
>
class ShaderProgramBase
{
private:
	using thisType = ShaderProgramBase<ShaderVertPathClass, ShaderFragPathClass>;
	Shader vertShader;
	Shader fragShader;
	ShaderProgram shaderProgram;
public:
	ShaderProgramBase():
		vertShader(GL_VERTEX_SHADER,   ShaderVertPathClass::getPath()),
		fragShader(GL_FRAGMENT_SHADER, ShaderFragPathClass::getPath()),
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

template
<
	typename ShaderVertPathClass,
	typename ShaderFragPathClass
>
void ShaderProgramBase<ShaderVertPathClass, ShaderFragPathClass>::init(void)
{
	vertShader.init();
	fragShader.init();
	
	vertShader.compile();
	fragShader.compile();
	
	shaderProgram.init();
	shaderProgram.attach(fragShader);
	shaderProgram.attach(vertShader);
	shaderProgram.link();
}


/*
class ShaderVertPathBasic
{
public:
	static std::string getPath()
	{
		return "basic.vert";
	}
}


class ShaderFragPathBasic
{
public:
	static std::string getPath()
	{
		return "basic.frag";
	}
}

using ShaderProgramPositionColor = ShaderProgramBase<ShaderVertPathBasic, ShaderFragPathBasic, VAOPositionColor, VAOPositionColorDynamic>;
*/
