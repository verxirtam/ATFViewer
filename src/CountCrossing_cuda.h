/*
 * =====================================================================================
 *
 *       Filename:  CountCrossing_host.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年06月05日 21時03分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef CountCrossing_host_H_
#define CountCrossing_host_H_

//////////////////////////////////////////////////////////////////////////////////////
// テンプレート関数の実体定義
//////////////////////////////////////////////////////////////////////////////////////

template <int D, int DI>//次元,交点を求める方向
__host__ __device__
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
__host__ __device__
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



//startかendに負の座標があるかを判定する
template <int D>//次元
__host__ __device__
bool hasNegativeElement
	(
		const float* const start,		//線分の始点
		const float* const end			//線分の終点
	)
{
	bool ret = false;
	for(int d = 0; d < D; d++)
	{
		if((start[d] < 0.0f) || (end[d] < 0.0f) )
		{
			ret = true;
		}
	}
	return ret;
}

//交点を求めるループのパラメーターを設定する
template <int DI>//次元,交点を求める方向
__host__ __device__
void setCountCrossingLoopParameters
	(
		const float* const start,//始点
		const float* const end,//終点
		const float* const interval,//区間の幅
		int* const ns,//始点のセルのインデックス
		int* const ne,//終点のセルのインデックス
		int* const is,//ループのインデックスの開始番号
		int* const ie,//ループのインデックスの終了番号
		int* const ioffset,//ループのインデックスと交点のインデックスの差
		int* const counteroffset//インクリメントするカウンタのインデックス
	)
{
	//始点の座標が終点の座標以下の場合
	if(start[DI] <= end[DI])
	{
		*ns = - floorf( - start[DI] / interval[DI] ) - 1;
		*ne = - floorf( -   end[DI] / interval[DI] ) - 1;
		*is = *ns + 1;
		*ie = *ne + 1;
		*ioffset = 0;
		*counteroffset = 0;
	}
	//始点の座標が終点の座標より大きい場合
	else
	{
		*ns = floorf( start[DI] / interval[DI] );
		*ne = floorf(   end[DI] / interval[DI] );
		*is = *ne;
		*ie = *ns;
		*ioffset = 1;
		*counteroffset =1;
	}
	
}

template <int D>
__host__ __device__
bool isCellIndexOutOfRange
	(
		const int* const cellindex,		//セルのインデックス
		const int* const startindex,	//カウンタのインデックスの開始番号
		const int* const indexcount		//インデックスの個数
	)
{
	//セルのインデックスがカウンタの範囲を超えていたら
	//カウンタのインクリメントを行わない
	bool ret = false;
	for(int d = 0; d < D; d++)
	{
		if((cellindex[d] < startindex[d]) || (startindex[d] + indexcount[d] <= cellindex[d]))
		{
			ret = true;
			break;
		}
	}
	return ret;
}

template <int D, int DI>
__host__ __device__
void getCellIndexFromPoint
	(
		const float* const point,		//セルインデックスを求める点
		const float* const interval,	//区間の幅
		int cellindex_di,				//DI方向のセルインデックス
		int* const cellindex			//セルインデックス
	)
{
		//加算するカウンタのセルのインデックスを算出する
		//第DI成分は交点ではなく指定のインデックスを使用する
		for(int d = 0; d < D; d++)
		{
			cellindex[d] = ( d == DI ) ? ( cellindex_di ) : ( std::floor( point[d] / interval[d] ) );
		}
	
}

//カウンタのインデックスを取得する
template <int D, int DI>//次元,交点を求める方向
__host__ __device__
int getCounterIndex
	(
		const int* const cellindex,		//セルインデックス
		const int* const startindex,	//カウンタのインデックスの開始番号
		const int* const indexcount,	//インデックスの個数
		int counteroffset				//カウンタのセル中のオフセット
	)
{
	//セルの通し番号
	int ci = getTotalCellIndex<D>(cellindex,startindex,indexcount);
	//1セルあたりのカウンタの個数を乗じる
	ci *= 2 * D;
	//セル中のカウンタのインデックスを加算する
	ci += 2 * DI + counteroffset;
	return ci;
}


template <int D, int DI>//次元,交点を求める方向
__host__
void countCrossingByDirectionHost
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
	
	//startかendに負の座標があれば終了する
	if(hasNegativeElement<D>(start,end))
	{
		return;
	}
	
	//交点を求めるループのパラメーターを設定する
	setCountCrossingLoopParameters<DI>(start,end,interval,&ns,&ne,&is,&ie,&ioffset,&counteroffset);
	
	for(int i = is; i < ie; i++)
	{
		//第DI座標が(i+ioffset)*interval[DI]の点を求める
		float p = (i+ioffset)*interval[DI];
		float cross[D];
		
		//交点を算出する
		getCrossingPoint<D,DI>(start,end,p,cross);
		
		//交点のセルインデックス
		int cellindex[D];
		
		//加算するカウンタのセルのインデックスを算出する
		//第DI成分は交点ではなく指定のインデックスを使用する
		getCellIndexFromPoint<D,DI>(cross, interval, i, cellindex);
		
		//セルのインデックスがカウンタの範囲を超えていたら
		//カウンタのインクリメントを行わない
		if(isCellIndexOutOfRange<D>(cellindex, startindex, indexcount))
		{
			continue;
		}
		
		//カウンタのインデックスを取得する
		int ci = getCounterIndex<D,DI>(cellindex,startindex,indexcount,counteroffset);
		
		//カウンタをインクリメントする
		counter[ci]++;
		
	}
}

//DI = 0〜D-1 についてcountCrossing()を実行するためにテンプレートの再帰を使用する
template <int D, int DI>//次元,交点を求める方向
struct countCrossingHostTemp
{
	__host__
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
		countCrossingHostTemp<D,DI-1>::imple(start,end,interval,startindex,indexcount,counter);
		//DI-1方向について平面との交点を求める
		countCrossingByDirectionHost<D, DI-1>(start,end,interval,startindex,indexcount,counter);
	}
};

//テンプレートの再帰がループにならずに終了するようにテンプレートの特殊化を行う
template <int D>
struct countCrossingHostTemp<D, 1>
{
	__host__
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
		countCrossingByDirectionHost<D,0>(start,end,interval,startindex,indexcount,counter);
	}
};

template <int D>//次元
__host__
void countCrossingHost
	(
		const float* const start,		//線分の始点
		const float* const end,			//線分の終点
		const float* const interval,	//区間の幅
		const int* const startindex,		//カウンタのインデックスの開始番号
		const int* const indexcount,	//インデックスの個数
		float* const counter		//区間の通過回数のカウンタ
	)
{
	countCrossingHostTemp<D,D>::imple(start,end,interval,startindex,indexcount,counter);
}

template <int D>//次元
__host__
void countCrossingSequenceHostImple
	(
		const float* const vertex,		//頂点の列
		int vertexequencecount,			//頂点の列の長さ
		const float* const interval,	//区間の幅
		const int* const startindex,		//カウンタのインデックスの開始番号
		const int* const indexcount,	//インデックスの個数
		float* const counter		//区間の通過回数のカウンタ
	)
{
	//バンクコンフリクト対策のパディング
	//Dが偶数->1
	//Dが機数->0
	int padding = (D + 1) & 1;
	int Dp = D + padding;
	int linecount = vertexequencecount / Dp - 1;
	for(int i = 0; i < linecount; i++)
	{
		int v = i * Dp;
		const float* const start = &(vertex[v]);
		const float* const  end = &(vertex[v + Dp]);
		countCrossingHost<D>(start,end,interval,startindex,indexcount,counter);
	}
}


template <int D, int LC>//次元, ブロックあたりの線分の個数
__global__
void countCrossingSequenceDeviceImple
	(
		const float* const vertex,		//頂点の列
		int vertexequencecount,			//頂点の列の長さ
		const float* const interval,	//区間の幅
		const int* const startindex,		//カウンタのインデックスの開始番号
		const int* const indexcount,	//インデックスの個数
		float* const counter		//区間の通過回数のカウンタ
	)
{
	//生成すべきブロック数
	int blockcount = ;
	//デバイスメモリを確保
	float* vertex_d;
	
	//デバイスへのメモリのコピー
	//カーネルの起動パラメータの設定
	
	//カーネルの起動
	//カーネルの結果をデバイスからコピー
}

#endif
