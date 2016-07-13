/*
 * =====================================================================================
 *
 *       Filename:  TestVBO.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月12日 01時17分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "TestVBO.h"

void TestVBO::display()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_POINTS, 0, 8);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

