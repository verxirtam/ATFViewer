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

#include "cudatestfunc.h"

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


void cudatestfunctest()
{
	float a[10] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
	int n = 10;
	
	for(int i = 0; i < n; i++)
	{
		cout << a[i];
	}
	cout << endl;
	
	cudatestfunc(a, n);
	
	for(int i = 0; i < n; i++)
	{
		cout << a[i];
	}
	cout << endl;
}

void countCrossingTest_01Simple()
{
	cout << "countCrossingTest_01Simple()" << endl;
	
	float start[1]={0.5f};
	float end[1]={1.5f};
	float interval[1]={1.0f};
	int startindex[1]={0};
	int indexcount[1]={2};
	float counter[4]={0.0f,0.0f,0.0f,0.0f};
	
	float cross[1];
	getCrossingPoint<1, 0>(start, end, 1.0f, cross);
	cout << "cross[0] = " << cross[0] << endl;
	
	int cellindex[1] = {1};
	int ci = getTotalCellIndex<1>(cellindex, startindex, indexcount);
	cout << "getTotalCellIndex<1>({1}, {0}, {2}) = 1 :  " << ci << endl;
	
	cellindex[0] = 0;
	ci = getTotalCellIndex<1>(cellindex, startindex, indexcount);
	cout << "getTotalCellIndex<1>({0}, {0}, {2}) = 0 :  " << ci << endl;
	
	cout << "countCrossingByDirection<1,0>(start, end, interval, startindex, indexcount, counter) :" << endl;
	countCrossingByDirection<1,0>(start, end, interval, startindex, indexcount, counter);
	for (int i = 0; i < 4; i++)
	{
		cout << counter[i] << '\t';
	}
	cout << endl;
	
	
	cout << "countCrossing<1>(start,end,interval,startindex,indexcount,counter) :" << endl;
	countCrossing<1>(start,end,interval,startindex,indexcount,counter);
	for (int i = 0; i < 4; i++)
	{
		cout << counter[i] << '\t';
	}
	cout << endl;
	
}

void countCrossingTest_02D1Long()
{
	
	cout << "countCrossingTest_02D1Long()" << endl;
	
	const int D = 1;
	const int L = 10;
	const int CL = L * D * 2;
	float start[D] = {7.5f};
	float end[D] = {9.5f};
	float interval[D] = {1.0f};
	int startindex[D] = {5};
	int indexcount[D] = {L};
	float counter[CL];
	for (int i = 0; i < CL; i++)
	{
		counter[i]=0.0f;
	}
	cout << "start : " << start[0] << endl;
	cout << "  end : " << end[0] << endl;
	
	countCrossing<1>(start,end,interval,startindex,indexcount,counter);
	for (int i = 0; i < L; i++)
	{
		int ci = i + startindex[0];
		cout << ci << '\t';
		cout << ci << '\t';
	}
	cout << endl;
	for (int i = 0; i < L; i++)
	{
		cout << 0 << '\t';
		cout << 1 << '\t';
	}
	cout << endl;
	for (int i = 0; i < L; i++)
	{
		cout << "--------";
		cout << "--------";
	}
	cout << endl;
	
	
	
	for (int i = 0; i < CL; i++)
	{
		cout << counter[i] << '\t';
	}
	cout << endl;
	
	start[0] = 11.5;
	end[0] = 14.0;
	countCrossing<1>(start,end,interval,startindex,indexcount,counter);
	for (int i = 0; i < CL; i++)
	{
		cout << counter[i] << '\t';
	}
	cout << endl;
	
	start[0] = 7.5;
	end[0] = 6.0;
	countCrossing<1>(start,end,interval,startindex,indexcount,counter);
	for (int i = 0; i < CL; i++)
	{
		cout << counter[i] << '\t';
	}
	cout << endl;
}


void countCrossingTest_03D2Simple()
{
	cout << "countCrossingTest_03D2Simple()" << endl;
	
	const int D = 2;
	const int L0 = 2;
	const int L1 = 4;
	const int CL = L0 * L1 * D * 2;
	float start[D] = {2.25f, 3.125f};
	float end[D] = {2.75f, 3.375f};
	float interval[D] = {1.0f, 0.5f};
	int startindex[D] = {2,6};
	int indexcount[D] = {L0,L1};
	float counter[CL];
	for (int i = 0; i < CL; i++)
	{
		counter[i]=0.0f;
	}
	
	cout << "start:(" << start[0] << ", " << start[1] << ")" << endl;
	cout << "  end:(" <<   end[0] << ", " <<   end[1] << ")" << endl;
	countCrossing<D>(start,end,interval,startindex,indexcount,counter);
	for (int i = 0; i < CL; i++)
	{
		cout << counter[i] << ' ';
	}
	cout << endl;
	
	start[0] = 2.5f;  start[1] = 3.5f;
	  end[0] = 3.25f;   end[1] = 4.75f;
	cout << "start:(" << start[0] << ", " << start[1] << ")" << endl;
	cout << "  end:(" <<   end[0] << ", " <<   end[1] << ")" << endl;
	countCrossing<D>(start,end,interval,startindex,indexcount,counter);
	for (int i = 0; i < CL; i++)
	{
		cout << counter[i] << ' ';
	}
	cout << endl;
	
	start[0] = 2.0f;  start[1] = 4.75f;
	  end[0] = 4.0f;    end[1] = 3.25f;
	cout << "start:(" << start[0] << ", " << start[1] << ")" << endl;
	cout << "  end:(" <<   end[0] << ", " <<   end[1] << ")" << endl;
	countCrossing<D>(start,end,interval,startindex,indexcount,counter);
	for (int i = 0; i < CL; i++)
	{
		cout << counter[i] << ' ';
	}
	cout << endl;
	
	start[0] = 4.25f;  start[1] = 4.0f;
	  end[0] = 3.5f;    end[1] = 5.25f;
	cout << "start:(" << start[0] << ", " << start[1] << ")" << endl;
	cout << "  end:(" <<   end[0] << ", " <<   end[1] << ")" << endl;
	countCrossing<D>(start,end,interval,startindex,indexcount,counter);
	for (int i = 0; i < CL; i++)
	{
		cout << counter[i] << ' ';
	}
	cout << endl;
	
	start[0] = 4.25f;  start[1] = 5.5f;
	  end[0] = 3.5f;    end[1] = 5.25f;
	cout << "start:(" << start[0] << ", " << start[1] << ")" << endl;
	cout << "  end:(" <<   end[0] << ", " <<   end[1] << ")" << endl;
	countCrossing<D>(start,end,interval,startindex,indexcount,counter);
	for (int i = 0; i < CL; i++)
	{
		cout << counter[i] << ' ';
	}
	cout << endl;
}

void countCrossingTest_04D2Seqence()
{
	
	cout << "countCrossingTest_04D2Seqence()" << endl;
	
	const int D = 2;
	const int L0 = 2;
	const int L1 = 4;
	const int VC = 10;
	const int CL = L0 * L1 * D * 2;
	float vertex[D * VC] = 
		{
			 2.5f,  3.25f,
			 3.5f,  3.25f,
			 3.5f,  3.75f,
			 2.75f, 4.25f,
			 2.5f,  4.75f,
			 3.0f,  5.25f,
			 3.5f,  4.75f,
			 3.5f,  4.25f,
			 2.5f,  3.75f,
			-1.0f, -1.0f
		};
	float interval[D] = {1.0f, 0.5f};
	int startindex[D] = {2,6};
	int indexcount[D] = {L0,L1};
	float counter[CL];
	for (int i = 0; i < CL; i++)
	{
		counter[i]=0.0f;
	}
	
	countCrossingSequence<D>(vertex,D * VC,interval,startindex,indexcount,counter);
	
	for(int i = 0; i < VC; i++)
	{
		for(int d = 0; d < D; d++)
		{
			int vi = d + i * D;
			cout << vertex[vi] << ", ";
		}
		cout << endl;
	}
	
	
	for (int i = 0; i < CL; i++)
	{
		cout << counter[i] << ' ';
	}
	cout << endl;
}

int main(int argc, char const* argv[])
{
	cout << nowstring() << " log: test start." << endl;
	
	//getAllTrackData();
	
	//getAllTrackDataReserve();
	
	//selectTrackDataWithTime();
	
	//selectTrackDataWithDate();
	
	//testTrackDataManager();
	
	//cudatestfunctest();
	
	countCrossingTest_01Simple();
	countCrossingTest_02D1Long();
	countCrossingTest_03D2Simple();
	countCrossingTest_04D2Seqence();
	
	cout << nowstring() << " log: test end." << endl;
	cout << endl;
	
	return 0;
}




