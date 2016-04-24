/*
 * =====================================================================================
 *
 *       Filename:  Path.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年04月17日 21時48分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef Path_H
#define Path_H

#include <vector>
#include <string>
#include <ctime>

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
	bool operator==(const std::string& s)
	{
		return this->id == s;
	}
	bool operator!=(const std::string& s)
	{
		return !((*this)==s);
	}
};

#endif
