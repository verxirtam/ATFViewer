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
	using vaoType = VAOPositionTexture<ShaderProgramTexture>;
	vaoType vao;
public:
	StringVAO():
		texture(GL_TEXTURE0),
		vao()
	{
	}
	void init()
	{
		texture.init("../../res/font/ExportedFont_Ricty_64_32.raw", 512, 512);
		float v[]
			=
			{
				0.0f, 1.0f,
				0.0f, 0.0f,
				1.0f, 1.0f,
				1.0f, 0.0f,
				2.0f, 1.0f,
				2.0f, 0.0f,
				3.0f, 1.0f,
				3.0f, 0.0f
			};
		vao.init();
	}
	void display()
	{
		vao.display();
	}
};


