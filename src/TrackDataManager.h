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
#include "Path.h"
#include "TimeManager.h"

class TrackDataManager
{
private:
	void getTrackDataFromDBWithDay(std::vector<Path>& paths,
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
		std::stringstream query("");
		query << "select id,longitude,latitude,altitude,time,arrival from TrackData where time >= ";
		query << start << " and time < ";
		query << end << " order by id,time;";
		dba.setQuery(query.str());

		//取得したTrackDataをpに追加
		//直前に読み込んだid
		//id毎にvectorに格納するため
		std::string old_id("");
		
		std::vector<Path>::iterator i = paths.begin();
		
		while(SQLITE_ROW == dba.step_select())
		{
			std::string id(dba.getColumnString(0));
			double lo=dba.getColumnDouble(1);
			double la=dba.getColumnDouble(2);
			int a=dba.getColumnInt(3);
			long long t=dba.getColumnLongLong(4);
			std::string ar(dba.getColumnString(5));
			
			//取得したidとi->idを照合
			if(id != old_id)
			{
				//idが一致するpathを検索
				i = std::find(paths.begin(),paths.end(),id);
				//検索で見つからなかった場合は新規追加
				if(i == paths.end())
				{
					//末尾に追加
					paths.push_back(Path());
					//最後の要素を指定
					i = paths.end();
					i--;
					//追加した要素を初期化
					i->id = id;
					i->past_time_index = 0;
					i->now_index = 0;
				}
				old_id = id;
			}
			i->pathPoint.push_back(PathPoint(lo,la,a,t,ar));
		}

	}
	
public:
	void getTrackDataFromDB(std::vector<Path>& p,
			time_t start,
			time_t end)
	{
		//開始日を求める
		time_t start_day = TimeManager::today(start);
		//日付：開始日から終了日までループ
		for(time_t d = start_day; d < end; d = TimeManager::nextDayTime(d))
		{
			getTrackDataFromDBWithDay(p, start, end, d);
		}
	}
};











#endif
