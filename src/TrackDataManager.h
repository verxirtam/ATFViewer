/*
 * =====================================================================================
 *
 *       Filename:  TrackDataManager.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年04月17日 08時24分25秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef TrackDataManager_H_
#define TrackDataManager_H_

#include <ctime>
#include <vector>
#include <sstream>
#include <algorithm>

#include "DBAccessor.h"
#include "PathPoint.h"
#include "TimeManager.h"

class TrackDataManager
{
private:
	void getTrackDataFromDBWithDay(std::vector<std::vector<PathPoint> >& p,
			std::vector<int>& past_time_index,
			std::vector<int>& now_index,
			time_t start,
			time_t end,
			time_t day)
	{
		//日付に対応するTrackDataのファイル名を生成
		char day_str[32];
		strftime(day_str, 31, "%Y%m%d", localtime(&day));
		std::stringstream dbfilename("");
		dbfilename << "../../db/TrackData/TrackData_";
		dbfilename << day_str;
		dbfilename << ".db";
		//ファイル名を指定してDBAccessorを生成
		DBAccessor dba(dbfilename.str());
		//クエリを実行
		stringstream query("");
		query << "select id,longitude,latitude,altitude,time,arrival from TrackData where time >= ";
		query << start << " and time < ";
		query << end << " order by id,time;";
		dba.setQuery(sql.str());

		//取得したTrackDataをpに追加
		//直前に読み込んだid
		//id毎にvectorに格納するため
		std::string old_id("");
		while(SQLITE_ROW == dba.step_select())
		{
			std::string id(dba.getColumnString(0));
			double lo=dba.getColumnDouble(1);
			double la=dba.getColumnDouble(2);
			int a=dba.getColumnInt(3);
			long long t=dba.getColumnLongLong(4);
			std::string ar(dba.getColumnString(5));
			//idで照合しないとPathを区別できない
		}

	}
	
public:
	void getTrackDataFromDB(std::vector<std::vector<PathPoint> >& p,
			std::vector<int>& past_time_index,
			std::vector<int>& now_index,
			time_t start,
			time_t end)
	{
		//開始日を求める
		time_t start_day = TimeManager::today(start);
		//日付：開始日から終了日までループ
		for(time_t d = start_day; d < end; d = TimeManager::nextDay(d))
		{
			getTrackDataFromDBWithDay(p, past_time_index, now_index, start, end, d)
		}
	}
};











#ifndef
