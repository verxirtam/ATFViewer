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
	typename ShaderFragPathClass,
	template<typename X> class VAOClass,
	template<typename Y> class VAODynamicClass
>
class ShaderProgramBase
{
private:
	using thisType = ShaderProgramBase<ShaderVertPathClass, ShaderFragPathClass, VAOClass, VAODynamicClass>;
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
	using vaoType        = VAOClass<thisType>;
	using vaoTypeDynamic = VAODynamicClass<thisType>;
};

template
<
	typename ShaderVertPathClass,
	typename ShaderFragPathClass,
	template<typename X> class VAOClass,
	template<typename Y> class VAODynamicClass
>
void ShaderProgramBase<ShaderVertPathClass, ShaderFragPathClass, VAOClass, VAODynamicClass>::init(void)
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
