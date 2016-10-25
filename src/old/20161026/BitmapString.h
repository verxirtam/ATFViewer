/*
 * =====================================================================================
 *
 *       Filename:  BitmapString.h
 *
 *    Description:  文字列を描画する
 *
 *        Version:  1.0
 *        Created:  2016年03月20日 20時03分07秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef BitmapString_H
#define BitmapString_H

#include <GL/glut.h>

class BitmapString
{
private:
	
public:
	//現在の位置に文字列を描画する
	//glutBitmapCharacter()はOpenGL3以降非推奨になっているので注意
	static void drawString(const char* str, void* font = GLUT_BITMAP_TIMES_ROMAN_10)
	{
		while(*str)
		{
			glutBitmapCharacter(font, *str);
			str++;
		}
	}
	//ワールド座標系上の(x,y,z)で指定された位置に文字列を描画する
	//glRasterPos3d()はOpenGL3以降非推奨になっているので注意
	static void drawString(double x, double y, double z, const char* str, void* font = GLUT_BITMAP_TIMES_ROMAN_10)
	{
		glRasterPos3d(x,y,z);
		drawString(str,font);
	}
	//ウィンドウ座標系上の(x,y)で指定された位置に文字列を描画する
	//glRasterPos2d()はOpenGL3以降非推奨になっているので注意
	static void drawString(double x, double y, const char* str, void* font = GLUT_BITMAP_TIMES_ROMAN_10)
	{
		glRasterPos2d(x,y);
		drawString(str,font);
	}
};

















#endif

