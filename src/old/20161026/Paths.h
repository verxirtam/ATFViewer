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

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "DBAccessor.h"
#include "TrackDataManager.h"
#include "Path.h"
#include "TimeSeparation.h"


class Paths
{
private:
	TimeSeparation timeSeparation;
	std::vector<Path> paths;
	std::vector<Path> pathsBuffer;
	std::vector<Path>* currentPaths;
	std::vector<Path>* bufferPaths;
	const unsigned int drawTimeWidth;
	std::future<void> futureMakeBuffer;
	
	void makePathsBuffer(std::vector<Path>& p, TimeSeparation::Position position);
	void runMakePathsBuffer(std::vector<Path>& p, TimeSeparation::Position position);
	void updatePastTimeIndex(Path& p, time_t past_time);
	void updateNowIndex(Path& p, time_t now);
	void drawPathLine(Path& p, time_t past_time, time_t now);
	//ポインタメンバを持っているが、コピー不要なので禁止する
	//コピーコンストラクタ
	Paths(const Paths&) = delete;
	//コピー代入演算子
	Paths& operator=(const Paths&) = delete;
	//ムーブコンストラクタ
	Paths(Paths&&) = delete;
	//ムーブ代入演算子
	Paths& operator=(Paths&&) = delete;
public:
	Paths()
		:
			timeSeparation(),
			paths(),
			pathsBuffer(),
			currentPaths(&paths),
			bufferPaths(&pathsBuffer),
			drawTimeWidth(600),
			futureMakeBuffer()
	{
	}
	void initPathPoint(time_t time_min, time_t time_max);
	void drawPath(PathPoint& p, time_t now);
	PathPoint getNowPoint(PathPoint& from, PathPoint& to, time_t time);
	void resetTime(void);
	int display(time_t now);
};









#endif

