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
#include "ShaderProgramTexture.h"

class StringVAO
{
private:
	Texture2D texture;
	using vaoType = ShaderProgramTexture::vaoType;
	vaoType vao;
public:
	StringVAO():
		texture(GL_TEXTURE0),
		vao()
	{

	}
};


