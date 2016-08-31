/*
 * =====================================================================================
 *
 *       Filename:  ShaderProgramPositionColor.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月31日 20時08分15秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ShaderProgramPositionColor.h"


void ShaderProgramPositionColor::init(void)
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
}
