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

#include <pthread.h>

#include <DBAccessor.h>
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

void setStartEndTime(time_t& start, time_t& end)
{
	tm start_tm;
	start_tm.tm_year = 2016 - 1900;
	start_tm.tm_mon  =  4 - 1;
	start_tm.tm_mday = 13;
	start_tm.tm_hour = 22;
	start_tm.tm_min  =  0;
	start_tm.tm_sec  =  0;
	tm end_tm = start_tm;
	end_tm.tm_hour += 4;
	start = mktime(&start_tm);
	end   = mktime(  &end_tm);
}

class Timer
{
private:
	double startTime;
	double endTime;
public:
	Timer():startTime(0.0),endTime(0.0)
	{
	}
	void start()
	{
		startTime = omp_get_wtime();
	}
	void end()
	{
		endTime = omp_get_wtime();
	}
	double getTime()
	{
		return endTime - startTime;
	}
};

bool testTrackDataManager()
{
	#pragma omp critical
	{
		cout << "testTrackDataManager()" << endl;
	}
	TrackDataManager tdm;
	std::vector<Path> p;
	time_t start;
	time_t end;
	setStartEndTime(start, end);
	
	Timer timer;
	timer.start();//double start_time = omp_get_wtime();
	tdm.getTrackDataFromDB(p,start,end);
	timer.end();//double end_time = omp_get_wtime();
	
	#pragma omp critical
	{
		cout << "vector版" << endl;
		cout << "経過時間 = " << timer.getTime() << "sec." << endl;
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
	Timer timer_map;
	timer_map.start();//double start_map_time = omp_get_wtime();
	tdm.getTrackDataFromDBToMap(mp, start, end);
	timer_map.end();//double end_map_time = omp_get_wtime();
	#pragma omp critical
	{
		cout << "map版" << endl;
		cout << "経過時間 = " << timer_map.getTime() << "sec." << endl;
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
	
	bool ret = true;
	
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
			bool ret_private = testTrackDataManager();
			#pragma omp critical
			{
				if(!ret_private)
				{
					cout << "test failed. thread index = " << i << endl;
					ret = ret_private;
				}
			}
			#pragma omp barrier
			#pragma omp single
			{
				cout << endl << endl << endl;
			}
		}
	}
	return ret;
}

bool MapParallelTest()
{
	bool ret = true;
	
	time_t start;
	time_t end;
	setStartEndTime(start, end);
	end += 6 * 60 * 60;
	TrackDataManager tdm;
	
	
	Timer timer_v;
	vector<Path> p;
	
	timer_v.start();
	tdm.getTrackDataFromDB(p, start, end);
	timer_v.end();
	
	cout << "timer_v.getTime() = " << timer_v.getTime() << endl;
	
	cout << "p.size() = " << p.size() << endl;
	
	
	Timer timer_mp;
	vector<map<string,Path> > p_mp;
	
	timer_mp.start();
	tdm.getTrackDataFromDBToMapParallel(p_mp, start, end);
	timer_mp.end();
	
	cout << "timer_mp.getTime() = " << timer_mp.getTime() << endl;
	unsigned int total_size = 0;
	cout << "p_mp.size() = " << p_mp.size() << endl;
	for(unsigned int i = 0;i < p_mp.size(); i++)
	{
		total_size += p_mp[i].size();
		cout << "p_mp[" << i << "].size() = " << p_mp[i].size() << endl;
	}
	
	cout << "total_size = " << total_size << endl;
	
	
	//pとmpが同じ内容かの確認
	if(p.size() != total_size)
	{
		return false;
	}
	int p_size = p.size();
	int p_mp_size = p_mp.size();
	for(int i = 0; i < p_size; i++)
	{
		bool found = false;
		for(int j = 0; j < p_mp_size; j++)
		{
			map<string,Path>::iterator it;
			it = p_mp[j].find(p[i].id);
			if(it != p_mp[j].end())
			{
				if(found)
				{
					ret = false;
					continue;
				}
				found = true;
				vector<PathPoint>& pp_p = p[i].pathPoint;
				vector<PathPoint>& pp_mp =  it->second.pathPoint;
				if(pp_p != pp_mp)
				{
					ret = false;
				}
			}
		}
	}
	
	
	
	
	return ret;
}


bool MapParallelTest2()
{
	cout << "MapParallelTest2()" << endl;
	
	
	bool ret = true;
	
	time_t start;
	time_t end;
	setStartEndTime(start, end);
	end += 48 * 60 * 60;
	TrackDataManager tdm;
	
	std::vector<Path> p[2];
	Timer timer[2];
	
	timer[0].start();
	tdm.getTrackDataFromDB(p[0], start, end);
	timer[0].end();
	
	timer[1].start();
	tdm.getTrackDataFromDBParallel(p[1], start, end, 6);
	timer[1].end();
	
	cout << "tdm.getTrackDataFromDB() time: "         << timer[0].getTime() << endl;
	cout << "tdm.getTrackDataFromDBParallel() time: " << timer[1].getTime() << endl;
	
	//順番が異なっているのでソートする
	std::sort(p[0].begin(), p[0].end(), comp_path);
	std::sort(p[1].begin(), p[1].end(), comp_path);
	
	//比較
	if(p[0] != p[1])
	{
		ret = false;
		if(p[0].size() != p[1].size())
		{
			cout << "p[0].size() = " << p[0].size() << endl;
			cout << "p[1].size() = " << p[1].size() << endl;
		}
		int p_size = p[0].size();
		for(int i = 0; i < p_size; i++)
		{
			if(p[0][i] != p[1][i])
			{
				cout << "p[0][" << i << "] != p[1][" << i << "]" << endl;
				cout << "p[0][" << i << "].id = " << p[0][i].id << endl;
				cout << "p[1][" << i << "].id = " << p[1][i].id << endl;
				cout << "p[0][" << i << "].past_time_index = " << p[0][i].past_time_index << endl;
				cout << "p[1][" << i << "].past_time_index = " << p[1][i].past_time_index << endl;
				cout << "p[0][" << i << "].now_index = " << p[0][i].now_index << endl;
				cout << "p[1][" << i << "].now_index = " << p[1][i].now_index << endl;
			}
		}
	}
	return ret;
}

typedef void (*myfunc)(void);

template <typename F>
class TestThread
{
private:
	pthread_t thread;
	F f;
	static void* _f(void* arg)
	{
		TestThread* t = (TestThread*)arg;
		t->f();
		return NULL;
	}
	/*
	void f()
	{
		cout << "This is test thread." << endl;
	}
	*/
public:
	TestThread(F func)
		:
			thread(),
			f(func)
	{
	}
	void run()
	{
		pthread_create(&thread, NULL, _f, (void*)this);
	}
	void join()
	{
		pthread_join(thread,NULL);
	}
};

struct TestThreadFunc
{
	void operator ()()
	{
		cout << "This is test thread." << endl;
	}
};

template <typename F, typename T>
class TestThread_MemFunc
{
private:
	pthread_t thread;
	F f;
	T* const t;
	static void* _f(void* arg)
	{
		TestThread_MemFunc* tt = (TestThread_MemFunc*)arg;
		((tt->t)->*(tt->f))();
		return NULL;
	}
	/*
	void f()
	{
		cout << "This is test thread." << endl;
	}
	*/
public:
	TestThread_MemFunc(F func, T* const ins):thread(),f(func),t(ins)
	{
	}
	void run()
	{
		pthread_create(&thread, NULL, _f, (void*)this);
	}
	void join()
	{
		pthread_join(thread,NULL);
	}
};

class TestUseThread
{
private:
	typedef void (TestUseThread::*functype)();
	TestThread_MemFunc<functype,TestUseThread> t;
	void function1()
	{
		cout << "This is function1." << endl;
	}
public:
	TestUseThread():t(&TestUseThread::function1, this)
	{
		
	}
	~TestUseThread()
	{
		this->t.join();
	}
	void exec()
	{
		this->t.run();
	}
};

bool testPThread()
{
	bool ret = true;
	
	TestThreadFunc f;
	TestThread<TestThreadFunc> t(f);
	t.run();
	
	t.join();
	
	
	TestUseThread tut;
	tut.exec();
	
	
	return ret;
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
	system("banner \"TEST\"");
	cout << nowstring() << " log: test start." << endl;
	
	bool ret = true;
	
	//getAllTrackData();
	
	//getAllTrackDataReserve();
	
	//selectTrackDataWithTime();
	
	//selectTrackDataWithDate();
	
	//test(testTrackDataManager(), ret);
	
	//cudatestfunctest();
	
	
	test(countCrossingTest_01Simple(), ret);
	test(countCrossingTest_02D1Long(), ret);
	test(countCrossingTest_03D2Simple(), ret);
	test(countCrossingTest_04D2Seqence(), ret);
	test(countCrossingTest_05Class(), ret);
	test(countCrossingTest_06D4EstimateSpec(), ret);
	
	
	test(openMPTest(), ret);
	
	test(MapParallelTest(), ret);
	test(MapParallelTest2(), ret);
	
	test(testPThread(), ret);
	
	test(PathsVAOTest_01isInInterval(), ret);
	test(PathsVAOTest_02updateElement(), ret);
	test(PathsVAOTest_03updateDeviceDataCUDAMain(), ret);
	
	
	if(ret)
	{
		//cout << "tests succeeded." << endl;
		system("banner \"success\"");
	}
	else
	{
		//cout << "tests failed." << endl;
		system("banner \"failed\"");
	}
	
	cout << nowstring() << " log: test end." << endl;
	cout << endl;
	
	return 0;
}




