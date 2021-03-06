/*
 * =====================================================================================
 *
 *       Filename:  TimeManager.h
 *
 *    Description:  時刻を管理する
 *
 *        Version:  1.0
 *        Created:  2016年04月17日 09時12分37秒
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

class TimeManager
{
private:
	
public:
	//引数の時刻の日付(00:00:00)をtime_t型で返却する
	static time_t today(time_t t)
	{
		tm* t_tm = localtime(&t);
		t_tm->tm_hour = 0;
		t_tm->tm_min = 0;
		t_tm->tm_sec = 0;
		return mktime(t_tm);
	}
	//引数の時刻の24時間後の日時をtime_t型で返却する
	static time_t nextDayTime(time_t t)
	{
		tm* t_tm = localtime(&t);
		t_tm->tm_mday += 1;
		return mktime(t_tm);
	}
};


