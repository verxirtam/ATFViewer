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
	shaderProgram.init();
	
	mvpMatrix.setLocation(shaderProgram.getHandle(), "mvpMatrix");
	
	textureSampler.setLocation(shaderProgram.getHandle(), "tex");
	textureSampler.set(0);
}

