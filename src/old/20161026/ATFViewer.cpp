/*
 * =====================================================================================
 *
 *       Filename:  ATFViewer.cpp
 *
 *    Description:  OpenGLで航空機の位置を表示する
 *
 *        Version:  1.0
 *        Created:  2016年01月24日 01時33分50秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  verxirtam 
 *   Organization:  
 *
 * =====================================================================================
 */


#include "ATFViewerMain.h"

int main(int argc, char const* argv[])
{
	ATFViewerMain& a = ATFViewerMain::getInstance();
	a.init();
	a.execMainLoop();
	return 0;
}

