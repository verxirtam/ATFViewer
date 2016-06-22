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
#include <map>
#include <algorithm>

#include <omp.h>

#include "DBAccessor.h"
#include "TrackDataManager.h"

#include "cudatestfunc.h"
#include "unittest_cuda.h"
#include "CountCrossing.h"

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

string timestring(time_t t)
{
	char time_str[32];
	//YYYY/MM/DD hh:mm:ss
	strftime(time_str, 31, "%Y%m%d%H%M%S", localtime(&t));
	std::string ret(time_str);
	return ret;
}


string nowstring()
{
	return timestring(time(NULL));
	/*
	time_t test_start = time(NULL);
	
	char time_str[32];
	//YYYY/MM/DD hh:mm:ss
	strftime(time_str, 31, "%Y%m%d%H%M%S", localtime(&test_start));
	std::string ret(time_str);
	return ret;
	*/
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

bool testTrackDataManager()
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
	
	double start_time = omp_get_wtime();
	tdm.getTrackDataFromDB(p,start,end);
	double end_time = omp_get_wtime();
	
	#pragma omp critical
	{
		cout << "経過時間 = " << (end_time - start_time) << "sec." << endl;
		cout << "取得件数:" << p.size() << endl;
		//時刻のチェック
		cout << "時刻のチェック" << endl;
	}
		std::sort(p.begin(),p.end(),comp_path_time);
	#pragma omp critical
	{
		cout << "最初のpathの開始時刻:" << timestring(p. begin()->pathPoint.begin()->time) << endl;
		cout << "最後のpathの開始時刻:" << timestring(p.rbegin()->pathPoint.begin()->time) << endl;
	
		//重複チェック
		cout << "重複チェック" << endl;
	}
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
	
	//map版との比較
	std::map<std::string, Path> mp;
	double start_map_time = omp_get_wtime();
	tdm.getTrackDataFromDBToMap(mp, start, end);
	double end_map_time = omp_get_wtime();
	#pragma omp critical
	{
		cout << "経過時間 = " << (end_map_time - start_map_time) << "sec." << endl;
		cout << "p.size() = " << p.size() << ", mp.size() = " << mp.size() << endl;
	}
	if(p.size() != mp.size())
	{
		success = false;
	}
	for(unsigned int i = 0; i < p.size(); i++)
	{
		std::map<std::string, Path>::iterator mi;
		mi = mp.find(p[i].id);
		if(mi == mp.end())
		{
			success = false;
		}
		else
		{
			Path& mpp = mi->second;
			if(mpp.id != p[i].id)
			{
				success = false;
			}
			if(mpp.past_time_index != p[i].past_time_index)
			{
				success = false;
			}
			if(mpp.now_index != p[i].now_index)
			{
				success = false;
			}
			if(mpp.pathPoint.size() != p[i].pathPoint.size())
			{
				success = false;
			}
			else
			{
				for(unsigned int j = 0; j < p[i].pathPoint.size(); j++)
				{
					if(mpp.pathPoint[j] != p[i].pathPoint[j])
					{
						success = false;
						continue;
					}
				}
			}
		}
	}
	
	
	
	#pragma omp critical
	{
		cout << (success?"成功":"失敗") << endl;
	}
	return success;
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
bool countCrossingTest_04D2Seqence()
{
	
	cout << "countCrossingTest_04D2Seqence()" << endl;
	
	bool ret = true;
	const int D = 2;
	const int L0 = 2;
	const int L1 = 4;
	const int VC = 10;
	const int CL = L0 * L1 * D * 2;
	float vertex[(D + 1) * VC] =
		{
			 2.5f,  3.25f, 0.0f,
			 3.5f,  3.25f, 0.0f,
			 3.5f,  3.75f, 0.0f,
			 2.75f, 4.25f, 0.0f,
			 2.5f,  4.75f, 0.0f,
			 3.0f,  5.25f, 0.0f,
			 3.5f,  4.75f, 0.0f,
			 3.5f,  4.25f, 0.0f,
			 2.5f,  3.75f, 0.0f,
			-1.0f, -1.0f,  0.0f
		};
	float interval[D] = {1.0f, 0.5f};
	int startindex[D] = {2,6};
	int indexcount[D] = {L0,L1};
	float counter[CL];
	float counter_d[CL];
	for (int i = 0; i < CL; i++)
	{
		counter[i]=0.0f;
		counter_d[i]=0.0f;
	}
	float result[CL] = {0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,2,0,0,0,0,1,1,0,0,1,0,0,0,0,1};
	
	CountCrossing::countCrossingSequenceHost(D, vertex, (D + 1) * VC, interval, startindex, indexcount, counter);
	
	for(int i = 0; i < VC; i++)
	{
		for(int d = 0; d < D + 1; d++)
		{
			int vi = d + i * (D + 1);
			cout << vertex[vi] << ", ";
		}
		cout << endl;
	}
	
	
	for (int i = 0; i < CL; i++)
	{
		cout << counter[i] << ' ';
		if(counter[i] != result[i])
		{
			ret = false;
		}
	}
	cout << endl;
	
	CountCrossing::countCrossingSequenceDevice(D, vertex, (D + 1) * VC, interval, startindex, indexcount, counter_d);
	
	for (int i = 0; i < CL; i++)
	{
		cout << counter_d[i] << ' ';
		if(counter_d[i] != result[i])
		{
			ret = false;
		}
	}
	cout << endl;
	
	return ret;
}

bool countCrossingTest_05Class()
{
	cout << "countCrossingTest_05Class()" << endl;
	
	CountCrossing cc;
	cc.init();
	time_t start_host = clock();
	cc.runOnHost();
	time_t end_host = clock();
	
	
	time_t start_device = clock();
	cc.runOnDevice();
	time_t end_device = clock();
	
	const vector<float> ch = cc.getCounter();
	const vector<float> cd = cc.getCounterDevice();
	
	if(ch.size() != cd.size())
	{
		return false;
	}
	int cmax = ch.size();
	for(int i = 0; i < cmax; i++)
	{
		if(ch[i] != cd[i])
		{
			return false;
		}
	}
	
	
	cout << "経過時間(host  ) = " << (double)(end_host   - start_host  ) / CLOCKS_PER_SEC << "sec." << endl;
	cout << "経過時間(device) = " << (double)(end_device - start_device) / CLOCKS_PER_SEC << "sec." << endl;
	
	return true;
	/*
	int imax = cc.getIndexCount(0);
	int jmax = cc.getIndexCount(1);
	int kmax = cc.getIndexCount(2);
	int lmax = cc.getIndexCount(3);
	for(int d = 0; d < 4; d++)
	{
		for(int s = 0; s < 2; s++)
		{
			for(int l = 0; l < lmax; l++)
			{
				for(int k = 0; k < kmax; k++)
				{
					for(int j = 0; j < jmax; j++)
					{
						for(int i = 0; i < imax; i++)
						{
							cout << cc.getCounterValue(d,s,i,j,k,l) << ", ";
						}
						cout << endl;
					}
					cout << endl;
				}
				cout << endl;
			}
			cout << endl << endl;
		}
		cout << endl << endl;
	}
	cout << endl << endl;
	*/
}
struct EstimateSpecResult
{
	int vertexCount;
	double initTime;
	double hostTime;
	double deviceTime;
	int counterCount;
	bool success;
};

void EstimateSpec(int hours, EstimateSpecResult& esr)
{
	CountCrossing cc;
	cc.setStart(2016,3,20,12        ,0,0);
	cc.setEnd  (2016,3,20,12 + hours,0,0);
	
	
	time_t start_init = clock();
	cc.init();
	time_t end_init = clock();
	
	
	esr.vertexCount = cc.getVertexCount();
	
	
	
	
	time_t start_host = clock();
	cc.runOnHost();
	time_t end_host = clock();
	
	time_t start_device = clock();
	cc.runOnDevice();
	time_t end_device = clock();
	
	esr.initTime   = (double)(end_init   - start_init  ) / CLOCKS_PER_SEC;
	esr.hostTime   = (double)(end_host   - start_host  ) / CLOCKS_PER_SEC;
	esr.deviceTime = (double)(end_device - start_device) / CLOCKS_PER_SEC;
	
	const vector<float>& ch = cc.getCounter();
	const vector<float>& cd = cc.getCounterDevice();
	
	esr.counterCount = ch.size();
	
	esr.success = true;
	if(ch.size() != cd.size())
	{
		esr.success = false;
	}
	int cmax = ch.size();
	for(int i = 0; i < cmax; i++)
	{
		if(ch[i] != cd[i])
		{
			esr.success = false;
		}
	}
}

bool countCrossingTest_06D4EstimateSpec()
{
	cout << "countCrossingTest_06D4EstimateSpec()" << endl;
	
	bool ret = true;
	int N = 10;
	int time[10] = {1,2,3,4,5,6,7,8,9,10};
	
	cout << "No" << '\t';
	cout << "時間幅" << '\t';
	cout << "頂点配列のサイズ（パディング含む）" << '\t';
	cout << "経過時間(init)" << '\t';
	cout << "経過時間(host)" << '\t';
	cout << "経過時間(device)" << '\t';
	cout << "カウンタのサイズ" << '\t';
	cout << "成否";
	cout << endl;
	
	for(int n = 0; n < N; n++)
	{
		EstimateSpecResult esr;
		EstimateSpec(time[n], esr);
		cout << n << '\t';
		cout << time[n] << '\t';
		cout << esr.vertexCount << '\t';
		cout << esr.initTime << '\t';
		cout << esr.hostTime << '\t';
		cout << esr.deviceTime << '\t';
		cout << esr.counterCount << '\t';
		cout << esr.success;
		cout << endl;
		
		if(esr.success == false)
		{
			ret = false;
		}
	}
	
	return ret;
}

bool openMPTest()
{
	for(int i = 0; i < 8; i++)
	{
		omp_set_num_threads(i+1);
		
		#pragma omp parallel
		{
			#pragma omp single
			{
				cout << "thread数:" << omp_get_num_threads() << endl;
			}
			#pragma omp critical
			{
				cout << "this is openmp test." << endl;
			}
			testTrackDataManager();
			#pragma omp barrier
			#pragma omp single
			{
				cout << endl << endl << endl;
			}
		}
	}
	return true;
}




void test(bool test_result, bool& ret)
{
	if(!test_result)
	{
		cout << "this test failed." << endl;
		ret = false;
	}
}

int main(int argc, char const* argv[])
{
	cout << nowstring() << " log: test start." << endl;
	
	bool ret = true;
	
	//getAllTrackData();
	
	//getAllTrackDataReserve();
	
	//selectTrackDataWithTime();
	
	//selectTrackDataWithDate();
	
	test(testTrackDataManager(), ret);
	
	//cudatestfunctest();
	
	/*
	test(countCrossingTest_01Simple(), ret);
	test(countCrossingTest_02D1Long(), ret);
	test(countCrossingTest_03D2Simple(), ret);
	test(countCrossingTest_04D2Seqence(), ret);
	test(countCrossingTest_05Class(), ret);
	test(countCrossingTest_06D4EstimateSpec(), ret);
	*/
	
	test(openMPTest(), ret);
	
	if(ret)
	{
		cout << "tests succeeded." << endl;
	}
	else
	{
		cout << "tests failed." << endl;
	}
	
	cout << nowstring() << " log: test end." << endl;
	cout << endl;
	
	return 0;
}




