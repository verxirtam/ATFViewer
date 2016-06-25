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
#include <map>
#include <sstream>
#include <algorithm>

#include "DBAccessor.h"
#include "Path.h"
#include "TimeManager.h"

class TrackDataManager
{
private:
	void makeDBFileName(time_t day, std::string& dbfilenamestr)
	{
		//日付に対応するTrackDataのファイル名を生成
		char day_str[32];
		strftime(day_str, 31, "%Y%m%d", localtime(&day));
		std::stringstream dbfilename("");
		dbfilename << "../../db/TrackData/TrackData_";
		dbfilename << day_str;
		dbfilename << ".db";
		dbfilenamestr =  dbfilename.str();
	}
	void setQuery(DBAccessor& dba, time_t start, time_t end, char n = ' ')
	{
		//クエリを実行
		std::stringstream query("");
		query << "select id,longitude,latitude,altitude,time,arrival from TrackData where time >= ";
		query << start << " and time < ";
		query << end;
		if(n != ' ')
		{
			query << " and substr(id,-1,1) = '" << n << "'";
			//std::cout << query.str() << std::endl;//TODO test
		}
		query << " order by id,time;";
		dba.setQuery(query.str());
	}

	void getColumn(DBAccessor& dba, std::string& id, double& lo, double& la, int& a, long long& t, std::string& ar)
	{
		id = dba.getColumnString(0);
		lo = dba.getColumnDouble(1);
		la = dba.getColumnDouble(2);
		a = dba.getColumnInt(3);
		t = dba.getColumnLongLong(4);
		ar = dba.getColumnString(5);
	}

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
	void getTrackDataFromDBToMapWithDay
		(
			std::map<std::string, Path>& paths,
			time_t start,
			time_t end,
			time_t day,
			char n = ' '
		)
	{
		//DBのファイル名
		std::string dbfilename;
		//dayに従ってアクセスするDBのファイル名を設定する
		makeDBFileName(day, dbfilename);
		//DBに接続
		DBAccessor dba(dbfilename);
		//クエリを実行
		setQuery(dba, start, end, n);
		
		//pathの追加先を指すイテレータ
		std::map<std::string, Path>::iterator i = paths.end();
		
		//クエリの結果の取得
		while(SQLITE_ROW == dba.step_select())
		{
			//1レコード分のカラムデータ
			std::string id;
			double lo;
			double la;
			int a;
			long long t;
			std::string ar;
			
			//カラムの取得
			getColumn(dba, id, lo, la, a, t, ar);
			
			//map::find()が要るかどうかを示す
			bool need_find = false;
			//iがend()を示すときはfind()を実行する
			if(i == paths.end())
			{
				need_find = true;
			}
			else
			{
				//iが異なるidの要素を指している場合はfind()を実行する
				if(i->first != id)
				{
					need_find = true;
				}
			}
			if(need_find)
			{
				//idを検索
				i = paths.find(id);
				//見つからなければ初期化したPathを追加
				if(i == paths.end())
				{
					//追加された要素を指すイテレータと追加の成否のペア
					std::pair<std::map<std::string, Path>::iterator, bool> ins;
					//要素の追加
					ins = paths.insert(std::make_pair(id, Path()));
					//findで見つからないことはわかっているので追加に成功した前提で継続
					//Pathの初期化
					ins.first->second.id = id;
					ins.first->second.past_time_index = 0;
					ins.first->second.now_index = 0;
					//iに追加したデータを指すように設定する
					i = ins.first;
				}
			}
			//取得したデータの追加
			i->second.pathPoint.push_back(PathPoint(lo,la,a,t,ar));
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
	void getTrackDataFromDBToMap(std::map<std::string, Path>& p, time_t start,time_t end)
	{
		//開始日を求める
		time_t start_day = TimeManager::today(start);
		//日付：開始日から終了日までループ
		for(time_t d = start_day; d < end; d = TimeManager::nextDayTime(d))
		{
			getTrackDataFromDBToMapWithDay(p, start, end, d);
		}
	}
	//OpenMPで並列化したgetTrackDataFromDBToMap()
	//mapからなるvectorに結果を格納する。vectorの要素が各スレッドに割り当てられる
	void getTrackDataFromDBToMapParallel(std::vector<std::map<std::string, Path> >& p, time_t start,time_t end)
	{
		//開始日を求める
		time_t start_day = TimeManager::today(start);
		
		const int N = 16;
		char id_last_char[N] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
		
		//結果格納用のコンテナのクリア
		p.clear();
		//コンテナの各要素の初期化
		for(int i = 0; i < N; i++)
		{
			p.push_back(std::map<std::string, Path>());
		}
		//これ以降が並列化
		#pragma omp parallel for
		for(int i = 0; i < N; i++)
		{
			std::cout << "id_last_char[i] = " << id_last_char[i] << std::endl;
			//日付：開始日から終了日までループ
			for(time_t d = start_day; d < end; d = TimeManager::nextDayTime(d))
			{
				getTrackDataFromDBToMapWithDay(p[i], start, end, d,id_last_char[i]);
			}
		}
	}
	
};











#endif
