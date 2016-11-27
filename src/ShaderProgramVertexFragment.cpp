/*
 * =====================================================================================
 *
 *       Filename:  ShaderProgramVertexFragment.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年11月28日 00時24分01秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ShaderProgramVertexFragment.h"

void ShaderProgramVertexFragment::init(void)
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




