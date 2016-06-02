/*
 * =====================================================================================
 *
 *       Filename:  cudatestfunc.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年05月17日 01時14分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef cudatestfunc_H_
#define cudatestfunc_H_

#include <cmath>



void cudatestfunc(float* a, int n);


template <int D, int DI>//次元,交点を求める方向
float*  getCrossingPoint
	(
		const float* const start,		//線分の始点
		const float* const end,			//線分の終点
		float x,			//交点を求める平面の座標
		float* const cross		//交点の出力先
	)
{
	//交点の線分のパラメータ
	float s = (x - start[DI]) / (end[DI] - start[DI]);
	//交点の算出
	//DI成分については誤差が出ないようにもともと求めていたxを使用する
	for(int d = 0; d < D; d++)
	{
		cross[d] = ( d == DI ) ? x : (end[d] - start[d]) * s + start[d];
	}
	//交点を返却
	return cross;
}

//セルのインデックス通し番号を算出する
template <int D>
int getTotalCellIndex
	(
		const int* const cellindex,	//カウンタのインデックス
		const int* const startindex,	//カウンタのインデックスの開始番号
		const int* const indexcount		//インデックスの個数
	)
{
	int ci = 0;
	for(int d = D - 1; d >= 0; d--)
	{
		ci *= indexcount[d];
		ci += cellindex[d] - startindex[d];
	}
	return ci;
}


template <int D, int DI>//次元,交点を求める方向
void countCrossingByDirection
	(
		const float* const start,		//線分の始点
		const float* const end,			//線分の終点
		const float* const interval,	//区間の幅
		const int* const startindex,	//カウンタのインデックスの開始番号
		const int* const indexcount,	//インデックスの個数
		float* const  counter			//区間の通過回数のカウンタ
	)
{
	int ns = 0;//始点のセルのインデックス
	int ne = 0;//終点のセルのインデックス
	int is = 0;//ループのインデックスの開始番号
	int ie = 0;//ループのインデックスの終了番号
	int ioffset = 0;//ループのインデックスと交点のインデックスの差
	int counteroffset = 0;//インクリメントするカウンタのインデックス
	
	//始点の座標が終点の座標以下の場合
	if(start[DI] <= end[DI])
	{
		ns = - std::floor( - start[DI] / interval[DI] ) - 1;
		ne = - std::floor( -   end[DI] / interval[DI] ) - 1;
		is = ns + 1;
		ie = ne + 1;
		ioffset = 0;
		counteroffset = 0;
	}
	//始点の座標が終点の座標より大きい場合
	else
	{
		ns = std::floor( start[DI] / interval[DI] );
		ne = std::floor(   end[DI] / interval[DI] );
		is = ne;
		ie = ns;
		ioffset = 1;
		counteroffset =1;
	}
	for(int i = is; i < ie; i++)
	{
		//第DI座標が(i+ioffset)*interval[DI]の点を求める
		float p = (i+ioffset)*interval[DI];
		float cross[D];
		
		//交点を算出する
		getCrossingPoint<D,DI>(start,end,p,cross);
		
		int cellindex[D];
		//加算するカウンタのセルのインデックスを算出する
		//第DI成分は交点ではなく指定のインデックスを使用する
		for(int d = 0; d < D; d++)
		{
			cellindex[d] = ( d == DI ) ? ( i ) : ( std::floor( cross[d] / interval[d] ) );
		}
		//セルのインデックスがカウンタの範囲を超えていたら
		//カウンタのインクリメントを行わない
		bool out_of_range = false;
		for(int d = 0; d < D; d++)
		{
			if((cellindex[d] < startindex[d]) || (startindex[d] + indexcount[d] <= cellindex[d]))
			{
				out_of_range = true;
				break;
			}
		}
		if(!out_of_range)
		{
			//セルの通し番号
			int ci = getTotalCellIndex<D>(cellindex,startindex,indexcount);
			//1セルあたりのカウンタの個数を乗じる
			ci *= 2 * D;
			//セル中のカウンタのインデックスを加算する
			ci += DI + counteroffset;
			//カウンタをインクリメントする
			counter[ci]++;
		}
	}
}

//DI = 0〜D-1 についてcountCrossing()を実行するためにテンプレートの再帰を使用する
template <int D, int DI>//次元,交点を求める方向
struct countCrossingTemp
{
	static void imple
		(
			const float* const start,		//線分の始点
			const float* const end,			//線分の終点
			const float* const interval,	//区間の幅
			const int* const startindex,		//カウンタのインデックスの開始番号
			const int* const indexcount,	//インデックスの個数
			float* const counter		//区間の通過回数のカウンタ
		)
	{
		//再帰呼出しを行いDI=0〜DI-2方向について実行する
		countCrossingTemp<D,DI-1>::imple(start,end,interval,startindex,indexcount,counter);
		//DI-1方向について平面との交点を求める
		countCrossingByDirection<D, DI-1>(start,end,interval,startindex,indexcount,counter);
	}
};

//テンプレートの再帰がループにならずに終了するようにテンプレートの特殊化を行う
template <int D>
struct countCrossingTemp<D, 1>
{
	static void imple
		(
			const float* const start,		//線分の始点
			const float* const end,			//線分の終点
			const float* const interval,	//区間の幅
			const int* const startindex,		//カウンタのインデックスの開始番号
			const int* const indexcount,	//インデックスの個数
			float* const counter		//区間の通過回数のカウンタ
		)
	{
		countCrossingByDirection<D,0>(start,end,interval,startindex,indexcount,counter);
	}
};

template <int D>//次元
void countCrossing
	(
		const float* const start,		//線分の始点
		const float* const end,			//線分の終点
		const float* const interval,	//区間の幅
		const int* const startindex,		//カウンタのインデックスの開始番号
		const int* const indexcount,	//インデックスの個数
		float* const counter		//区間の通過回数のカウンタ
	)
{
		countCrossingTemp<D,D>::imple(start,end,interval,startindex,indexcount,counter);
}





#endif

