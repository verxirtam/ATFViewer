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
#include <algorithm>

#include <GL/glut.h>

#include "DBAccessor.h"
#include "TrackDataManager.h"
#include "Path.h"

class Paths
{
private:
	std::vector<Path> paths;
	const unsigned int drawTimeWidth;
	void updatePastTimeIndex(Path& p, time_t past_time);
	void updateNowIndex(Path& p, time_t now);
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

