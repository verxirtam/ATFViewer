/*
 * =====================================================================================
 *
 *       Filename:  PathsVAO.h
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

#pragma once


#include <ctime>
#include <vector>
#include <sstream>
#include <algorithm>
#include <future>

#include "OpenGLHeaders.h"

#include "DBAccessor.h"
#include "TrackDataManager.h"
#include "Path.h"
#include "TimeSeparation.h"
#include "BasicShaderProgram.h"

class PathsVAO
{
private:
	using vaoType = BasicShaderProgram::vaoTypeDynamic;
	
	TimeSeparation timeSeparation;
	//std::vector<Path> paths;
	std::vector<Path> pathsBuffer;
	//std::vector<Path>* currentPaths;
	//std::vector<Path>* bufferPaths;
	const unsigned int drawTimeWidth;
	std::future<void> futureMakeBuffer;
	vaoType vao;
	
	void makePathsBuffer(std::vector<Path>& p, TimeSeparation::Position position);
	void runMakePathsBuffer(std::vector<Path>& p, TimeSeparation::Position position);
	void updatePastTimeIndex(Path& p, time_t past_time);
	void updateNowIndex(Path& p, time_t now);
	void drawPathLine(Path& p, time_t past_time, time_t now);
	void initVAO(const std::vector<Path>& p, vaoType& v);
	//ポインタメンバを持っているが、コピー不要なので禁止する
	//コピーコンストラクタ
	PathsVAO(const PathsVAO&) = delete;
	//コピー代入演算子
	PathsVAO& operator=(const PathsVAO&) = delete;
	//ムーブコンストラクタ
	PathsVAO(PathsVAO&&) = delete;
	//ムーブ代入演算子
	PathsVAO& operator=(PathsVAO&&) = delete;
public:
	PathsVAO(BasicShaderProgram& bsp)
		:
			timeSeparation(),
			//paths(),
			pathsBuffer(),
			//currentPaths(&paths),
			//bufferPaths(&pathsBuffer),
			drawTimeWidth(600),
			futureMakeBuffer(),
			vao(bsp)
	{
	}
	void initPathPoint(time_t time_min, time_t time_max);
	void drawPath(PathPoint& p, time_t now);
	PathPoint getNowPoint(PathPoint& from, PathPoint& to, time_t time);
	void resetTime(void);
	int display(time_t now);
};










