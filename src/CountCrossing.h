/*
 * =====================================================================================
 *
 *       Filename:  CountCrossing.h
 *
 *    Description:  航空機の通過機数を集計する
 *
 *        Version:  1.0
 *        Created:  2016年06月05日 19時24分04秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */



#pragma once

#include "TrackDataManager.h"

class CountCrossing
{
private:
	time_t start;
	time_t end;
	std::vector<float> vertex;
	std::vector<float> interval;
	std::vector<int> startIndex;
	std::vector<int> indexCount;
	std::vector<float> counter;
	std::vector<float> counter_device;
	
public:
	static void countCrossingSequenceHost
		(
			int dimension,			//次元
			const float* const vertex,	//頂点の列
			int vertexequencecount,		//頂点の列の長さ
			const float* const interval,	//区間の幅
			const int* const startindex,	//カウンタのインデックスの開始番号
			const int* const indexcount,	//インデックスの個数
			float* const counter		//区間の通過回数のカウンタ
		);
	static void countCrossingSequenceDevice
		(
			int dimension,			//次元
			const float* const vertex,	//頂点の列
			int vertexequencecount,		//頂点の列の長さ
			const float* const interval,	//区間の幅
			const int* const startindex,	//カウンタのインデックスの開始番号
			const int* const indexcount,	//インデックスの個数
			float* const counter		//区間の通過回数のカウンタ
		);
	time_t mktimeFromInt(int year,int month,int day,int hour = 0,int min = 0, int sec = 0)
	{
		std::tm time_tm;
		time_tm.tm_year = year - 1900;
		time_tm.tm_mon  = month - 1;
		time_tm.tm_mday = day;
		time_tm.tm_hour = hour;
		time_tm.tm_min  = min;
		time_tm.tm_sec  = sec;
		
		return  mktime(&time_tm);
	}
public:
	CountCrossing():
		start(1456801200),
		end(start + 60 * 60 * 1),
		vertex(),
		interval(),
		startIndex(),
		indexCount(),
		counter(),
		counter_device()
	{
		interval.push_back(1.0f);//1deg.
		interval.push_back(1.0f);//1deg.
		interval.push_back(40000.0f);//40000feet
		interval.push_back( 3600.0f);//60min
		
		startIndex.push_back(110);//longitude
		startIndex.push_back( 10);//latitude
		startIndex.push_back(  0);//altitude
		startIndex.push_back(  0);//time
		
		indexCount.push_back(60);
		indexCount.push_back(50);
		indexCount.push_back( 1);
		indexCount.push_back( 1);
		
		int counter_size = indexCount[0] * indexCount[1] * indexCount[2] * indexCount[3] * 4 * 2;
		counter = std::vector<float>(counter_size, 0.0f);
	}
	void init(void);
	void runOnHost(void);
	void runOnDevice(void);
	void setStart(int year,int month,int day,int hour = 0,int min = 0, int sec = 0)
	{
		start = mktimeFromInt(year, month, day, hour, min, sec);
	}
	void setStart(time_t s)
	{
		start = s;
	}
	time_t getStart()
	{
		return start;
	}
	void setEnd(int year,int month,int day,int hour = 0,int min = 0, int sec = 0)
	{
		end = mktimeFromInt(year, month, day, hour, min, sec);
	}
	void setEnd(time_t e)
	{
		end = e;
	}
	time_t getEnd()
	{
		return end;
	}
	int getVertexCount()
	{
		return vertex.size();
	}
	const std::vector<float>& getCounter()
	{
		return counter;
	}
	const std::vector<float>& getCounterDevice()
	{
		return counter_device;
	}
	int getIndexCount(int i)
	{
		return indexCount[i];
	}
	float getCounterValue(int d, int s, int cx, int cy, int cz, int ct)
	{
		int i = cx + indexCount[0] * (cy + indexCount[1] * ( cz + indexCount[2] * ct ));
		i *= 2 * 4;
		i += 2 * d + s;
		return counter[i];
	}
};



