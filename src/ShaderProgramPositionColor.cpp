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
	shaderProgram.init();
	
	mvpMatrix.setLocation(shaderProgram.getHandle(), "mvpMatrix");
}

