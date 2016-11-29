/*
 * =====================================================================================
 *
 *       Filename:  StringVAO.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年11月28日 01時30分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Texture2D.h"
#include "VAOPositionTexture.h"
#include "ShaderProgramTexture.h"

class StringVAO
{
private:
	Texture2D texture;
	ShaderProgramTexture shaderProgram;
	using vaoType = VAOPositionTexture<ShaderProgramTexture>;
	vaoType vao;
public:
	StringVAO():
		texture(GL_TEXTURE0),
		shaderProgram(),
		vao(shaderProgram, texture)
	{
	}
	void init()
	{
		shaderProgram.init();
		glm::mat4 m = glm::mat4(1.0f);
		shaderProgram.setMVPMatrix(m);
		
		texture.init("../../res/font/ExportedFont_Ricty_64_32.raw", 512, 512);
		
		vaoType::inputType input;
		input.position
			=
			{
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 0.0f,
				1.0f, 0.0f, 0.0f
			};
		std::vector<unsigned int> e{0,1,2,3};
		char s='A';
		
		int n = s-32;
		int i = (n % 16) * 32;
		int j = (n / 16) * 64;
		float u0 = static_cast<float>(i     ) / 512.0f;
		float v0 = static_cast<float>(j     ) / 512.0f;
		float u1 = static_cast<float>(i + 32) / 512.0f;
		float v1 = static_cast<float>(j + 64) / 512.0f;
		
		input.texture
			=
			{
				u0, v0,
				u0, v1,
				u1, v0,
				u1, v1
			};
		
		vao.init(input, e, GL_TRIANGLE_STRIP);

	}
	void display()
	{
		vao.display();
	}
};


