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
#include "CountCrossing_cuda.h"

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
	countCrossingByDirection<1,0>(start, end, interval, startindex, indexcount, counter);
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
	countCrossing<1>(start,end,interval,startindex,indexcount,counter);
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
		if(counter[i] != result[0][i])
		{
			ret = false;
		}
	}
	cout << endl;
	
	start[0] = 11.5;
	end[0] = 14.0;
	countCrossing<1>(start,end,interval,startindex,indexcount,counter);
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
	countCrossing<1>(start,end,interval,startindex,indexcount,counter);
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
	float start[D] = {2.25f, 3.125f};
	float end[D] = {2.75f, 3.375f};
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
	countCrossing<D>(start,end,interval,startindex,indexcount,counter);
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
	countCrossing<D>(start,end,interval,startindex,indexcount,counter);
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
	countCrossing<D>(start,end,interval,startindex,indexcount,counter);
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
	countCrossing<D>(start,end,interval,startindex,indexcount,counter);
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
	countCrossing<D>(start,end,interval,startindex,indexcount,counter);
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
