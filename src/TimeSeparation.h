/*
 * =====================================================================================
 *
 *       Filename:  TimeSeparation.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月04日 21時51分43秒
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


class TimeSeparation
{
private:
	time_t min;
	time_t max;
	time_t width;
	std::vector< time_t > timeSequence;
	unsigned int currentIndex;
	//指定した区間に時刻tが含まれているか判定する
	bool inTheInterval(unsigned int i, time_t t)
	{
		return (timeSequence[i] <= t) && (t < timeSequence[i + 1]);
	}
	//currentの次の区間のインデックスを取得する
	unsigned int getNextIndex()
	{
		return getNextIndex(currentIndex);
	}
	//次の区間のインデックスを取得する
	//currentが末尾の区間の場合は最初の区間のインデックスを取得する
	unsigned int getNextIndex(unsigned int i)
	{
		if(i + 1 >= timeSequence.size() - 1)
		{
			return 0;
		}
		return i + 1;
	}
	//開始・終了時刻を区間幅からtimeSequenceを初期化する
	void initTimeSequence()
	{
		timeSequence.push_back(min);
		for(time_t t = min + width; t < max - width; t += width)
		{
			timeSequence.push_back(t);
		}
		timeSequence.push_back(max);
	}
public:
	//currentか、nextかを示す列挙型
	enum Position
	{
		current = 0,
		next = 1
	};
	//コンストラクタ
	//デフォルト引数はデフォルトコンストラクタとするための仮の値で意味は無い
	TimeSeparation(time_t _min = 0, time_t _max = 10, time_t _width = 100):min(_min), max(_max), width(_width), timeSequence()
	{
		initTimeSequence();
		currentIndex = 0;
	}
	//currentの区間に含まれているかを判定する
	bool inCurrentInterval(time_t t)
	{
		return inTheInterval(currentIndex, t);
	}
	//次の区間に含まれているか判定する
	bool inNextInterval(time_t t)
	{
		return inTheInterval(getNextIndex(currentIndex), t);
	}
	//指定した区間の開始・終了時刻を取得する
	void getIntervalStatus(Position p,time_t& start, time_t& end)
	{
		switch(p)
		{
		case Position::current:
			std::cout << "TimeSeparation::getIntervalStatus() current." << std::endl;
			start = timeSequence[currentIndex    ];
			end   = timeSequence[currentIndex + 1];
			break;
		case Position::next:
			std::cout << "TimeSeparation::getIntervalStatus() next." << std::endl;
			start = timeSequence[getNextIndex()    ];
			end   = timeSequence[getNextIndex() + 1];
			break;
		}
	}
	void setNextInterval()
	{
		currentIndex = getNextIndex();
	}
};
