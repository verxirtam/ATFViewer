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
		int dimension,				//次元
		const float* const vertex,		//頂点の列
		int vertexequencecount,			//頂点の列の長さ
		const float* const interval,	//区間の幅
		const int* const startindex,		//カウンタのインデックスの開始番号
		const int* const indexcount,	//インデックスの個数
		float* const counter		//区間の通過回数のカウンタ
	)
{
	switch (dimension)
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
