/*
 * =====================================================================================
 *
 *       Filename:  glunittest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月13日 18時38分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */




#include "./ATFViewerMainGLSL.h"

int main(void)
{
	ATFViewerMainGLSL& a(ATFViewerMainGLSL::getInstance());
	
	a.init();
	
	a.execMainLoop();
	
	return 0;
}
