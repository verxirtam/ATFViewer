/*
 * =====================================================================================
 *
 *       Filename:  GLCall.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年12月11日 16時47分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#pragma once


#include <iostream>

//GL関数の実行時エラーをチェック・表示するためのマクロ
#define CHECK_GL_CALL(msg)\
{\
	GLenum e = glGetError();\
	if(e != GL_NO_ERROR)\
	{\
		std::cout << "GL_ERROR : " << gluErrorString(e) << " at ";\
		std::cout << __FILE__ << " ";\
		std::cout << __LINE__ << " ";\
		std::cout << #msg << std::endl;\
	}\
}

#define GL_CALL(cmd)\
	cmd;\
	CHECK_GL_CALL(#cmd);

