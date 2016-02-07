/*
 * =====================================================================================
 *
 *       Filename:  Map.h
 *
 *    Description:  マップを描画する
 *
 *        Version:  1.0
 *        Created:  2016年02月07日 17時23分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef MAP_H_
#define MAP_H_

#include <cstdio>
#include <cstdlib>

#include <GL/glut.h>

class Map
{
private:
	GLdouble centerOffsetLong;
	GLdouble centerOffsetLat;
	GLdouble scale;
	
public:
	Map():
		centerOffsetLong(0.0),
		centerOffsetLat(0.0),
		scale(1.0)
	{
	}
	void init(void);
	void display(void);
	void setCenterOffsetLong(GLdouble c_long)
	{
		centerOffsetLong = (c_long < -180.0)?(-180.0):
					((c_long > 180.0)?180.0:c_long);
	}
	GLdouble getCenterOffsetLong(void)
	{
		return centerOffsetLong;
	}
	void setCenterOffsetLat(GLdouble c_lat)
	{
		centerOffsetLat = (c_lat < -90.0)?(-90.0):
					((c_lat > 90.0)?90.0:c_lat);
	}
	GLdouble getCenterOffsetLat(void)
	{
		return centerOffsetLat;
	}
	void setScale(GLdouble s)
	{
		scale = (s > 0.0) ? s : 1.0;
	}
	GLdouble getScale(void)
	{
		return scale;
	}
};





#endif
