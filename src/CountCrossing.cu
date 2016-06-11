/*
 * =====================================================================================
 *
 *       Filename:  CountCrossing.cu
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年06月05日 21時32分42秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CountCrossing.h"
#include "CountCrossing_cuda.h"


void CountCrossing::countCrossingSequenceHost
	(
		int dimension,					//次元
		const float* const vertex,		//頂点の列
		int vertexequencecount,			//頂点の列の長さ
		const float* const interval,	//区間の幅
		const int* const startindex,		//カウンタのインデックスの開始番号
		const int* const indexcount,	//インデックスの個数
		float* const counter		//区間の通過回数のカウンタ
	)
{
	switch(dimension)
	{
	case 1:
		countCrossingSequenceHostImple<1>(vertex,vertexequencecount,interval,startindex,indexcount,counter);
		break;
	case 2:
		countCrossingSequenceHostImple<2>(vertex,vertexequencecount,interval,startindex,indexcount,counter);
		break;
	case 3:
		countCrossingSequenceHostImple<3>(vertex,vertexequencecount,interval,startindex,indexcount,counter);
		break;
	case 4:
		countCrossingSequenceHostImple<4>(vertex,vertexequencecount,interval,startindex,indexcount,counter);
		break;
	}
}

void CountCrossing::init(void)
{
	TrackDataManager tdm;
	std::vector<Path> path;
	time_t start = 1456801200;
	time_t end = start + 60 * 60 * 1;
	
	tdm.getTrackDataFromDB(path, start, end);
	
	int imax = path.size();
	for(int i = 0;i < imax; i++)
	{
		int jmax = path[i].pathPoint.size();
		for(int j = 0; j < jmax; j++)
		{
			vertex.push_back(path[i].pathPoint[j].longitude);
			vertex.push_back(path[i].pathPoint[j].latitude);
			vertex.push_back(path[i].pathPoint[j].altitude);
			time_t t = path[i].pathPoint[j].time;
			float time = (float)(t - start);
			vertex.push_back(time);
			//パディング
			vertex.push_back(0.0f);
		}
		vertex.push_back(-1.0f);
		vertex.push_back(-1.0f);
		vertex.push_back(-1.0f);
		vertex.push_back(-1.0f);
		vertex.push_back(-1.0f);
	}

}

void CountCrossing::runOnHost(void)
{
	int counter_size = indexCount[0] * indexCount[1] * indexCount[2] * indexCount[3] * 4 * 2;
	counter = std::vector<float>(counter_size, 0.0f);
	
	CountCrossing::countCrossingSequenceHost
		(
			4,
			vertex.data(),
			vertex.size(),
			interval.data(),
			startIndex.data(),
			indexCount.data(),
			counter.data()
		);
}

void CountCrossing::runOnDevice(void)
{
	int counter_size = indexCount[0] * indexCount[1] * indexCount[2] * indexCount[3] * 4 * 2;
	counter_device = std::vector<float>(counter_size, 0.0f);
	
	CountCrossing::countCrossingSequenceHost
		(
			4,
			vertex.data(),
			vertex.size(),
			interval.data(),
			startIndex.data(),
			indexCount.data(),
			counter_device.data()
		);
}

