/*
 * =====================================================================================
 *
 *       Filename:  unittest_cuda.cu
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年06月09日 00時20分57秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include "unittest_cuda.h"
#include "CountCrossing.cuh"

#include "PathsVAO_cuda_kernel.cuh"


using namespace std;

bool countCrossingTest_01Simple()
{
	cout << "countCrossingTest_01Simple()" << endl;
	
	bool ret = true;
	
	float start[1]={0.5f};
	float end[1]={1.5f};
	float interval[1]={1.0f};
	int startindex[1]={0};
	int indexcount[1]={2};
	float counter[4]={0.0f,0.0f,0.0f,0.0f};
	float result[2][4] = 
		{
			{0,0,1,0},
			{0,0,2,0}
		};
	
	float cross[1];
	getCrossingPoint<1, 0>(start, end, 1.0f, cross);
	cout << "cross[0] = " << cross[0] << endl;
	if(cross[0] != 1.0f)
	{
		ret = false;
	}
	
	int cellindex[1] = {1};
	int ci = getTotalCellIndex<1>(cellindex, startindex, indexcount);
	cout << "getTotalCellIndex<1>({1}, {0}, {2}) = 1 :  " << ci << endl;
	if(ci != 1)
	{
		ret = false;
	}
	
	cellindex[0] = 0;
	ci = getTotalCellIndex<1>(cellindex, startindex, indexcount);
	cout << "getTotalCellIndex<1>({0}, {0}, {2}) = 0 :  " << ci << endl;
	if(ci != 0)
	{
		ret = false;
	}
	
	cout << "countCrossingByDirection<1,0>(start, end, interval, startindex, indexcount, counter) :" << endl;
	countCrossingByDirectionHost<1,0>(start, end, interval, startindex, indexcount, counter);
	for (int i = 0; i < 4; i++)
	{
		cout << counter[i] << '\t';
		if(counter[i] != result[0][i])
		{
			ret = false;
		}
	}
	cout << endl;
	
	
	cout << "countCrossing<1>(start,end,interval,startindex,indexcount,counter) :" << endl;
	countCrossingHost<1>(start,end,interval,startindex,indexcount,counter);
	for (int i = 0; i < 4; i++)
	{
		cout << counter[i] << '\t';
		if(counter[i] != result[1][i])
		{
			ret = false;
		}
	}
	cout << endl;
	return ret;
}


bool countCrossingTest_02D1Long()
{
	
	cout << "countCrossingTest_02D1Long()" << endl;
	
	bool ret = true;
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
	float result[3][CL] = 
		{
			{0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0},
			{0,0,0,1,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0}
		};
	
	
	
	cout << "start : " << start[0] << endl;
	cout << "  end : " << end[0] << endl;
	
	countCrossingHost<1>(start,end,interval,startindex,indexcount,counter);
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
		if(counter[i] != result[0][i])
		{
			ret = false;
		}
	}
	cout << endl;
	
	start[0] = 11.5;
	end[0] = 14.0;
	countCrossingHost<1>(start,end,interval,startindex,indexcount,counter);
	for (int i = 0; i < CL; i++)
	{
		cout << counter[i] << '\t';
		if(counter[i] != result[1][i])
		{
			ret = false;
		}
	}
	cout << endl;
	
	start[0] = 7.5;
	end[0] = 6.0;
	countCrossingHost<1>(start,end,interval,startindex,indexcount,counter);
	for (int i = 0; i < CL; i++)
	{
		cout << counter[i] << '\t';
		if(counter[i] != result[2][i])
		{
			ret = false;
		}
	}
	cout << endl;
	return ret;
}


bool countCrossingTest_03D2Simple()
{
	cout << "countCrossingTest_03D2Simple()" << endl;
	
	const int D = 2;
	const int L0 = 2;
	const int L1 = 4;
	const int CL = L0 * L1 * D * 2;
	float start[D + 1] = {2.25f, 3.125f, 0.0f};
	float end[D + 1] = {2.75f, 3.375f, 0.0f};
	float interval[D] = {1.0f, 0.5f};
	int startindex[D] = {2,6};
	int indexcount[D] = {L0,L1};
	float counter[CL];
	float result[5][CL] = 
		{
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,1,2,0,0,0,1,0,0,0,0,0,1,0},
			{0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,1,2,1,0,0,1,0,0,0,0,0,2,0},
			{0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,1,2,1,0,0,1,0,0,0,0,0,2,0}
		};
	for (int i = 0; i < CL; i++)
	{
		counter[i]=0.0f;
	}
	
	bool ret = true;
	
	int n = 0;
	
	cout << "start:(" << start[0] << ", " << start[1] << ")" << endl;
	cout << "  end:(" <<   end[0] << ", " <<   end[1] << ")" << endl;
	countCrossingHost<D>(start,end,interval,startindex,indexcount,counter);
	for (int i = 0; i < CL; i++)
	{
		cout << counter[i] << ' ';
		if(counter[i] != result[n][i])
		{
			ret = false;
			cout << "error at " << ": ";
			cout << "counter[" << i << "]=" << counter[i];
			cout << "!=" << "result[" << n << "][" << i << "]=" << result[n][i];
		}
	}
	cout << endl;
	
	n = 1;
	start[0] = 2.5f;  start[1] = 3.5f;
	  end[0] = 3.25f;   end[1] = 4.75f;
	cout << "start:(" << start[0] << ", " << start[1] << ")" << endl;
	cout << "  end:(" <<   end[0] << ", " <<   end[1] << ")" << endl;
	countCrossingHost<D>(start,end,interval,startindex,indexcount,counter);
	for (int i = 0; i < CL; i++)
	{
		cout << counter[i] << ' ';
		if(counter[i] != result[n][i])
		{
			ret = false;
			cout << "error at " << ": ";
			cout << "counter[" << i << "]=" << counter[i];
			cout << "!=" << "result[" << n << "][" << i << "]=" << result[n][i];
		}
	}
	cout << endl;
	
	n = 2;
	start[0] = 2.0f;  start[1] = 4.75f;
	  end[0] = 4.0f;    end[1] = 3.25f;
	cout << "start:(" << start[0] << ", " << start[1] << ")" << endl;
	cout << "  end:(" <<   end[0] << ", " <<   end[1] << ")" << endl;
	countCrossingHost<D>(start,end,interval,startindex,indexcount,counter);
	for (int i = 0; i < CL; i++)
	{
		cout << counter[i] << ' ';
		if(counter[i] != result[n][i])
		{
			ret = false;
			cout << "error at " << ": ";
			cout << "counter[" << i << "]=" << counter[i];
			cout << "!=" << "result[" << n << "][" << i << "]=" << result[n][i];
		}
	}
	cout << endl;
	
	n = 3;
	start[0] = 4.25f;  start[1] = 4.0f;
	  end[0] = 3.5f;    end[1] = 5.25f;
	cout << "start:(" << start[0] << ", " << start[1] << ")" << endl;
	cout << "  end:(" <<   end[0] << ", " <<   end[1] << ")" << endl;
	countCrossingHost<D>(start,end,interval,startindex,indexcount,counter);
	for (int i = 0; i < CL; i++)
	{
		cout << counter[i] << ' ';
		if(counter[i] != result[n][i])
		{
			ret = false;
			cout << "error at " << ": ";
			cout << "counter[" << i << "]=" << counter[i];
			cout << "!=" << "result[" << n << "][" << i << "]=" << result[n][i];
		}
	}
	cout << endl;
	
	n = 4;
	start[0] = 4.25f;  start[1] = 5.5f;
	  end[0] = 3.5f;    end[1] = 5.25f;
	cout << "start:(" << start[0] << ", " << start[1] << ")" << endl;
	cout << "  end:(" <<   end[0] << ", " <<   end[1] << ")" << endl;
	countCrossingHost<D>(start,end,interval,startindex,indexcount,counter);
	for (int i = 0; i < CL; i++)
	{
		cout << counter[i] << ' ';
		if(counter[i] != result[n][i])
		{
			ret = false;
			cout << "error at " << ": ";
			cout << "counter[" << i << "]=" << counter[i];
			cout << "!=" << "result[" << n << "][" << i << "]=" << result[n][i];
		}
	}
	cout << endl;

	return ret;
}



void _test(bool test_result, bool& ret)
{
	if(!test_result)
	{
		cout << "this test failed." << endl;
		ret = false;
	}
}

bool PathsVAOTest_01isInInterval()
{
	bool ret = true;
	
	std::vector<float> vertex
		{// x      y      z      t     r       g       b       a       p
			0.0f, 10.0f, 20.0f, 30.0f, 0.000f, 0.100f, 0.200f, 0.300f, 0.0f,// 0   0 = past
			0.5f, 10.5f, 20.5f, 30.0f, 0.005f, 0.105f, 0.205f, 0.305f, 0.0f,// 1   9
			1.0f, 11.0f, 21.0f, 31.0f, 0.010f, 0.110f, 0.210f, 0.310f, 0.0f,// 2  18 =  now
			1.5f, 11.5f, 21.5f, 31.0f, 0.015f, 0.115f, 0.215f, 0.315f, 0.0f,// 3  27
			2.0f, 12.0f, 22.0f, 32.0f, 0.020f, 0.120f, 0.220f, 0.320f, 0.0f,// 4  36 = begin_iindex
			2.5f, 12.5f, 22.5f, 32.0f, 0.025f, 0.125f, 0.225f, 0.325f, 0.0f,// 5  45
			3.0f, 13.0f, 23.0f, 33.0f, 0.030f, 0.130f, 0.230f, 0.330f, 0.0f,// 6  54
			3.5f, 13.5f, 23.5f, 33.0f, 0.035f, 0.135f, 0.235f, 0.335f, 0.0f,// 7  63
			4.0f, 14.0f, 24.0f, 34.0f, 0.040f, 0.140f, 0.240f, 0.340f, 0.0f,// 8  72
			4.5f, 14.5f, 24.5f, 34.0f, 0.045f, 0.145f, 0.245f, 0.345f, 0.0f,// 9  81
			5.0f, 15.0f, 25.0f, 35.0f, 0.050f, 0.150f, 0.250f, 0.350f, 0.0f,//10  90
			5.5f, 15.5f, 25.5f, 35.0f, 0.055f, 0.155f, 0.255f, 0.355f, 0.0f //11  99
		};//                                                                //12 108 = end_iindex
	unsigned int begin_index  =   4;
	unsigned int   end_index  =  12;
	unsigned int begin_iindex =  36;
	unsigned int   end_iindex = 108;
	
	vector<float> vertex1(vertex);
	
	_test(!PathsVAO_isInInterval(30.5f, vertex1.data(), begin_iindex, end_iindex,  9), ret);
	
	_test(!PathsVAO_isInInterval(31.0f, vertex1.data(), begin_iindex, end_iindex, 36), ret);
	_test( PathsVAO_isInInterval(32.0f, vertex1.data(), begin_iindex, end_iindex, 36), ret);
	_test( PathsVAO_isInInterval(32.5f, vertex1.data(), begin_iindex, end_iindex, 36), ret);
	_test(!PathsVAO_isInInterval(33.0f, vertex1.data(), begin_iindex, end_iindex, 36), ret);
	_test(!PathsVAO_isInInterval(33.5f, vertex1.data(), begin_iindex, end_iindex, 36), ret);
	
	_test( PathsVAO_isInInterval(34.0f, vertex1.data(), begin_iindex, end_iindex, 72), ret);
	_test( PathsVAO_isInInterval(34.5f, vertex1.data(), begin_iindex, end_iindex, 72), ret);
	_test(!PathsVAO_isInInterval(35.0f, vertex1.data(), begin_iindex, end_iindex, 72), ret);
	_test(!PathsVAO_isInInterval(35.5f, vertex1.data(), begin_iindex, end_iindex, 72), ret);
	
	_test(!PathsVAO_isInInterval(35.5f, vertex1.data(), begin_iindex, end_iindex, 99), ret);
	_test(!PathsVAO_isInInterval(35.5f, vertex1.data(), begin_iindex, end_iindex,108), ret);
	
	
	vector<float> vertex2{vertex};
	float* const past_vertex2(vertex2.data() +  0);
	float* const  now_vertex2(vertex2.data() + 18);
	
	
	unsigned int vii = 36;
	PathsVAO_updateTimeVertex(32.5f, vertex2.data(), begin_iindex, end_iindex, vii, past_vertex2);
	//_test();
	// 2.0f, 12.0f, 22.0f, 32.0f, 0.020f, 0.120f, 0.220f, 0.320f, 0.0f,// 4  36 = begin_iindex
	// 3.0f, 13.0f, 23.0f, 33.0f, 0.030f, 0.130f, 0.230f, 0.330f, 0.0f,// 6  54
	// ---------------------------------------------------------------
	// 2.5,  12.5,  22.5,  32.5,  0.025,  0.125,  0.225,  0.325
	for(int i = 0; i < 18; i++)
	{
		if(past_vertex2[i] != ( vertex2[vii + i] + vertex2[vii + 18 + i] ) * 0.5f)
		{
			ret = false;
		}
	}
	
	
	vii = 72;
	PathsVAO_updateTimeVertex(34.5f, vertex2.data(), begin_iindex, end_iindex, vii, past_vertex2);
	for(int i = 0; i < 18; i++)
	{
		if(past_vertex2[i] != ( vertex2[vii + i] + vertex2[vii + 18 + i] ) * 0.5f)
		{
			ret = false;
		}
	}
	
	vector<float> vertex3{vertex};
	float* const past_vertex3(vertex3.data() +  0);
	float* const  now_vertex3(vertex3.data() + 18);
	unsigned int time_index = 4;
	cout << "time_index = " << time_index << ", past_vertex3[0] = " << past_vertex3[0] << endl;
	
	cout << "PathsVAO_updateTimeIndex();" << endl;
	PathsVAO_updateTimeIndex(32.5f, vertex3.data(), begin_index, end_index, &time_index, past_vertex3);
	
	cout << "time_index = " << time_index << ", past_vertex3[0] = " << past_vertex3[0] << endl;
	_test(time_index      == 4   , ret);
	_test(past_vertex3[0] == 2.5f, ret);
	
	PathsVAO_updateTimeIndex(33.5f, vertex3.data(), begin_index, end_index, &time_index, past_vertex3);
	_test(time_index      == 6   , ret);
	_test(past_vertex3[0] == 3.5f, ret);
	
	cout << "time = 35.5f" << endl;
	PathsVAO_updateTimeIndex(35.5f, vertex3.data(), begin_index, end_index, &time_index, past_vertex3);
	cout << "time_index = " << time_index << ", past_vertex3[0] = " << past_vertex3[0] << endl;
	_test(time_index      == 12   , ret);
	_test(past_vertex3[0] ==  3.5f, ret);
	
	cout << "time = 31.5f" << endl;
	PathsVAO_updateTimeIndex(31.5f, vertex3.data(), begin_index, end_index, &time_index, past_vertex3);
	cout << "time_index = " << time_index << ", past_vertex3[0] = " << past_vertex3[0] << endl;
	_test(time_index      ==  2   , ret);
	_test(past_vertex3[0] ==  0.0f, ret);
	
	
	return ret;
}


