/*
 * =====================================================================================
 *
 *       Filename:  unittest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年04月09日 23時22分31秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "DBAccessor.h"
#include "TrackDataManager.h"

using namespace std;


struct id_time
{
	string id;
	long long time;
};



void addTrackData(vector<id_time>& r, DBAccessor& dba)
{
	//クエリを登録
	dba.setQuery("select id, time from TrackData;");
	
	//クエリの実行して結果を1行ずつ取得
	while(SQLITE_ROW == dba.step_select())
	{
		id_time it={dba.getColumnString(0), dba.getColumnLongLong(1)};
		
		//vectorに追加
		r.push_back(it);
	}
}

void getAllTrackData(void)
{
	
	cout << "事前のreserve()無し" << endl;
	
	vector<id_time> r;
	
	DBAccessor dba("../../db/ATFViewer.db");
	
	//時間計測開始
	clock_t start = clock();
	
	//結果の取得・格納
	addTrackData(r, dba);
	
	//時間計測完了
	clock_t end = clock();
	
	cout << "取得行数：" << r.size() << endl;

	cout << "経過時間 = " << (double)(end - start) / CLOCKS_PER_SEC << "sec." << endl;
	cout << endl;
}

void getAllTrackDataReserve(void)
{
	
	cout << "事前のreserve()有り" << endl;
	
	vector<id_time> r;
	
	DBAccessor dba("../../db/ATFViewer.db");
	
	//時間計測開始
	clock_t start = clock();
	
	//サイズの事前取得
	dba.setQuery("select count(*) from TrackData;");
	int size = 0;
	//クエリの実行して結果を1行ずつ取得
	while(SQLITE_ROW == dba.step_select())
	{
		size = dba.getColumnInt(0);
	}

	//サイズ取得完了時間計測
	clock_t count_end = clock();
	
	//reserve(容量確保)
	r.reserve(size);
	
	//容量確保完了時間計測
	clock_t reserve_end = clock();
	
	//結果の取得・格納
	addTrackData(r, dba);
	
	//時間計測完了
	clock_t end = clock();
	
	cout << "取得行数：" << r.size() << endl;

	cout << "サイズ取得時間 = " << (double)(count_end - start) / CLOCKS_PER_SEC << "sec." << endl;
	cout << "容量確保時間 = " << (double)(reserve_end - count_end) / CLOCKS_PER_SEC << "sec." << endl;
	cout << "結果取得時間 = " << (double)(end - reserve_end) / CLOCKS_PER_SEC << "sec." << endl;
	cout << "全体経過時間 = " << (double)(end - start) / CLOCKS_PER_SEC << "sec." << endl;
	
	cout << endl;
}

string nowstring()
{
	time_t test_start = time(NULL);
	
	char time_str[32];
	//YYYY/MM/DD hh:mm:ss
	strftime(time_str, 31, "%Y%m%d%H%M%S", localtime(&test_start));
	std::string ret(time_str);
	return ret;
}


void selectTrackDataWithTime()
{
	cout << "時刻指定でのトラックデータ取得" << endl;
	
	//時間計測
	clock_t start = clock();
	
	DBAccessor dba("../../db/ATFViewer.db");
	long long now = 1456153155;
	stringstream query("");
	query << "select id, time from TrackData where time >= ";
	query << now;
	query << " and time < ";
	query << now + 60 * 60 * 24;
	query << ";";
	dba.setQuery(query.str());

	vector<id_time> r;
	
	//クエリの実行して結果を1行ずつ取得
	while(SQLITE_ROW == dba.step_select())
	{
		id_time it={dba.getColumnString(0), dba.getColumnLongLong(1)};
		
		//vectorに追加
		r.push_back(it);
	}
	//時間計測
	clock_t end = clock();
	cout << "実行したクエリ：" << query.str() << endl;
	cout << "取得行数：" << r.size() << endl;

	cout << "経過時間 = " << (double)(end - start) / CLOCKS_PER_SEC << "sec." << endl;
	cout << endl;
}



void selectTrackDataWithDate()
{
	cout << "日付指定でのトラックデータ取得" << endl;
	
	//時間計測
	clock_t start = clock();
	
	DBAccessor dba("../../db/TrackData/TrackData_20160214.db");
	std::tm now_tm;
	now_tm.tm_year = 2016 - 1900;
	now_tm.tm_mon  =  2 - 1;
	now_tm.tm_mday = 13;
	now_tm.tm_hour = 32;
	now_tm.tm_min  =  0;
	now_tm.tm_sec  =  0;

	time_t now = mktime(&now_tm);
	stringstream query("");
	query << "select id, time from TrackData where time >= ";
	query << now;
	query << " and time < ";
	query << now + 60 * 60 * 2;
	query << ";";
	dba.setQuery(query.str());

	vector<id_time> r;
	
	//クエリの実行して結果を1行ずつ取得
	while(SQLITE_ROW == dba.step_select())
	{
		id_time it={dba.getColumnString(0), dba.getColumnLongLong(1)};
		
		//vectorに追加
		r.push_back(it);
	}
	//時間計測
	clock_t end = clock();
	cout << "実行したクエリ：" << query.str() << endl;
	cout << "取得行数：" << r.size() << endl;

	cout << "経過時間 = " << (double)(end - start) / CLOCKS_PER_SEC << "sec." << endl;
	cout << endl;
}

bool comp_path_time(const Path& p0, const Path& p1)
{
	return (p0.pathPoint.begin()->time) < (p1.pathPoint.begin()->time);
}

bool comp_path(const Path& p0, const Path& p1)
{
	return p0.id < p1.id;
}

void testTrackDataManager()
{
	cout << "testTrackDataManager()" << endl;
	
	TrackDataManager tdm;
	std::vector<Path> p;
	tm start_tm;
	start_tm.tm_year = 2016 - 1900;
	start_tm.tm_mon  =  4 - 1;
	start_tm.tm_mday = 13;
	start_tm.tm_hour = 22;
	start_tm.tm_min  =  0;
	start_tm.tm_sec  =  0;
	tm end_tm = start_tm;
	end_tm.tm_hour += 4;
	time_t start = mktime(&start_tm);
	time_t end   = mktime(  &end_tm);
	tdm.getTrackDataFromDB(p,start,end);
	cout << "取得件数:" << p.size() << endl;
	//時刻のチェック
	cout << "時刻のチェック" << endl;
	std::sort(p.begin(),p.end(),comp_path_time);
	cout << "最初のpathの開始時刻:" << ctime(&(p. begin()->pathPoint.begin()->time)) << endl;
	cout << "最後のpathの開始時刻:" << ctime(&(p.rbegin()->pathPoint.begin()->time)) << endl;
	//重複チェック
	cout << "重複チェック" << endl;
	std::sort(p.begin(),p.end(),comp_path);
	bool success = true;
	std::string id("XXXXXXXXXXXXXXXXXXX");
	for(std::vector<Path>::iterator i = p.begin(); i != p.end(); i++)
	{
		if(id==i->id)
		{
			success = false;
			cout << "failure:" << id << "!=" << i->id << endl;
		}
		id = i->id;
	}
	cout << (success?"成功":"失敗") << endl;
}

int main(int argc, char const* argv[])
{
	cout << nowstring() << " log: test start." << endl;
	
	//getAllTrackData();
	
	//getAllTrackDataReserve();
	
	//selectTrackDataWithTime();
	
	//selectTrackDataWithDate();
	
	testTrackDataManager();
	
	cout << nowstring() << " log: test end." << endl;
	cout << endl;
	
	return 0;
}




