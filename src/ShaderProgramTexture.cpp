/*
 * =====================================================================================
 *
 *       Filename:  ShaderProgramTexture.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月08日 03時27分30秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ShaderProgramTexture.h"


void ShaderProgramTexture::init(void)
{
	vertShader.init();
	fragShader.init();
	
	vertShader.compile();
	fragShader.compile();
	
	shaderProgram.init();
	shaderProgram.attach(fragShader);
	shaderProgram.attach(vertShader);
	shaderProgram.link();
	
	mvpMatrix.setLocation(shaderProgram.getHandle(), "mvpMatrix");
	
	textureSampler.setLocation(shaderProgram.getHandle(), "tex");
	textureSampler.set(0);
}

