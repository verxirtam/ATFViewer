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
	string ret(::ctime(&test_start));
	replace(ret.begin(), ret.end(), '\n', '\0');
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
	query << " and time <";
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

int main(int argc, char const* argv[])
{
	cout << nowstring() << " log: test start." << endl;
	
	getAllTrackData();
	
	getAllTrackDataReserve();
	
	selectTrackDataWithTime();
	
	cout << nowstring() << " log: test end." << endl;
	cout << endl;
	
	return 0;
}




