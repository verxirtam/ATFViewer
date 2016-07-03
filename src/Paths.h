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
#include <future>

#include <GL/glut.h>

#include "DBAccessor.h"
#include "TrackDataManager.h"
#include "Path.h"

class Paths
{
private:
	time_t timeWidth;
	time_t timeMin;
	time_t timeMax;
	std::vector<time_t> timeSeparation;
	time_t currentTimeSeparationIndex;
	std::vector<Path>* currentPaths;
	std::vector<Path>* bufferPaths;
	std::vector<Path> paths;
	std::vector<Path> pathsBuffer;
	const unsigned int drawTimeWidth;
	std::future<void> futureMakeBuffer;
	void makeTimeSeparation();
	void makePathsBuffer(std::vector<Path>& p, int time_separation_index);
	void runMakePathsBuffer(std::vector<Path>& p, int time_separation_index);
	void updatePastTimeIndex(Path& p, time_t past_time);
	void updateNowIndex(Path& p, time_t now);
	void drawPathLine(Path& p, time_t past_time, time_t now);
public:
	Paths():timeWidth(2 * 60 * 60), drawTimeWidth(600)
	{
	}
	void initPathPoint(DBAccessor& dba, time_t time_min, time_t time_max);
	void drawPath(PathPoint& p, time_t now);
	PathPoint getNowPoint(PathPoint& from, PathPoint& to, time_t time);
	void resetTime(void);
	int display(time_t now);
};









#endif

