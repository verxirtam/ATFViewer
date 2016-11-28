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
#include "VAOPaths.h"
#include "ShaderProgramPaths.h"

#include "DeviceSeq.cuh"

class PathsVAO
{
private:
	using vaoType = VAOPaths<ShaderProgramPaths>;
	struct doubleBufferingType
	{
		vaoType vao;
		DeviceMemory<unsigned int> indexListDevice;
		doubleBufferingType(ShaderProgramPaths& spp):vao(spp),indexListDevice()
		{
		}
	};
	
	
	
	TimeSeparation timeSeparation;
	//std::vector<Path> paths;
	//std::vector<Path> pathsBuffer;
	//std::vector<Path>* currentPaths;
	//std::vector<Path>* bufferPaths;
	const unsigned int drawTimeWidth;
	std::future<void> futureMakeBuffer;
	//vaoType vao0;
	//vaoType vao1;
	//vaoType* vaoCurrent;
	//vaoType* vaoBuffer;
	
	//std::vector<unsigned int> indexList;
	//DeviceMemory<unsigned int> indexListDevice0;
	//DeviceMemory<unsigned int> indexListDevice1;
	//DeviceMemory<unsigned int>* indexListDeviceCurrent;
	//DeviceMemory<unsigned int>* indexListDeviceBuffer;
	doubleBufferingType doubleBuffer0;
	doubleBufferingType doubleBuffer1;
	doubleBufferingType* doubleBufferingCurrent;
	doubleBufferingType* doubleBufferingBuffer;
	
	enum indexListItem
	{
		beginIndex = 0,
		pastIndex = 1,
		nowIndex =2,
		elementBeginIndex = 3
	};
	unsigned int getIndexListIndex(unsigned int path_index, indexListItem i)
	{
		return 3 * path_index + i;
	}
	
	void makePathsBuffer(doubleBufferingType& db, TimeSeparation::Position position);
	void runMakePathsBuffer(doubleBufferingType& db, TimeSeparation::Position position);
	void updatePastTimeIndex(Path& p, time_t past_time);
	void updateNowIndex(Path& p, time_t now);
	void drawPathLine(Path& p, time_t past_time, time_t now);
	void initVAO(const std::vector<Path>& p, doubleBufferingType& db, time_t time_start, time_t time_end);
	void updateDeviceData(time_t now);
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
	PathsVAO(ShaderProgramPaths& spp)
		:
			timeSeparation(),
			//pathsBuffer(),
			drawTimeWidth(600),
			futureMakeBuffer(),
			//vao0(bsp),
			//vao1(bsp),
			//vaoCurrent(&vao0),
			//vaoBuffer( &vao1),
			//indexList(),
			//indexListDevice0(),
			//indexListDevice1(),
			//indexListDeviceCurrent(&indexListDevice0),
			//indexListDeviceBuffer(&indexListDevice1)
			doubleBuffer0(spp),
			doubleBuffer1(spp),
			doubleBufferingCurrent(&doubleBuffer0),
			doubleBufferingBuffer(&doubleBuffer1)
	{
	}
	~PathsVAO()
	{
		//メンバを破棄する前に裏スレッドが完了するまで待機する
		//(破棄したメンバを更新しないようにするため)
		futureMakeBuffer.wait();
	}
	void initPathPoint(time_t time_min, time_t time_max);
	void drawPath(PathPoint& p, time_t now);
	PathPoint getNowPoint(PathPoint& from, PathPoint& to, time_t time);
	void resetTime(void);
	int display(time_t now);
};










