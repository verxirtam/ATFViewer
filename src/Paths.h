/*
 * =====================================================================================
 *
 *       Filename:  Paths.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年03月27日 21時53分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef Paths_H
#define Paths_H


#include <ctime>
#include <vector>
#include <sstream>

#include <GL/glut.h>

#include "DBAccessor.h"


struct PathPoint
{
	double longitude;
	double latitude;
	int altitude;
	time_t time;
	std::string arrival;
	PathPoint(double lo,double la, int a, long long t, const std::string ar)
	{
		longitude = lo;
		latitude = la;
		altitude = a;
		time = t;
		arrival = ar;
	}
};

struct Path
{
	std::string id;
	std::vector<PathPoint> pathPoint;
	int past_time_index;
	int now_index;
};

class Paths
{
private:
	std::vector<std::vector<PathPoint> > paths;
	std::vector<int> past_time_index;
	std::vector<int> now_index;
	const unsigned int drawTimeWidth;
public:
	Paths():drawTimeWidth(600)
	{
	}
	void initPathPoint(DBAccessor& dba, time_t time_min, time_t time_max);
	void drawPath(PathPoint& p, time_t now);
	PathPoint getNowPoint(PathPoint& from, PathPoint& to, time_t time);
	void resetTime(void);
	void display(time_t now);
};










#endif

