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
#pragma once

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
		:
			longitude(lo),
			latitude(la),
			altitude(a),
			time(t),
			arrival(ar)
	{
	}
	bool operator ==(const PathPoint& p) const
	{
		if(this->longitude != p.longitude)
		{
			return false;
		}
		if(this->latitude != p.latitude)
		{
			return false;
		}
		if(this->altitude != p.altitude)
		{
			return false;
		}
		if(this->time != p.time)
		{
			return false;
		}
		if(this->arrival != p.arrival)
		{
			return false;
		}
		return true;
	}
	bool operator !=(const PathPoint& p) const
	{
		return !((*this) == p);
	}
};

struct Path
{
	std::string id;
	std::vector<PathPoint> pathPoint;
	int past_time_index;
	int now_index;
	Path()
		:
			id(),
			pathPoint(),
			past_time_index(0),
			now_index(0)
	{
	}
	bool operator ==(const std::string& s) const
	{
		return this->id == s;
	}
	bool operator !=(const std::string& s) const
	{
		return !((*this)==s);
	}
	bool operator ==(const Path& p) const
	{
		return (this->id == p.id)
			&& (this->pathPoint == p.pathPoint)
			&& (this->past_time_index == p.past_time_index)
			&& (this->now_index == p.now_index);
	}
	bool operator !=(const Path& p) const
	{
		return !((*this)==p);
	}
};

