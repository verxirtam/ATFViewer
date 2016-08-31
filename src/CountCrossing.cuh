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

#include <vector>
#include <cuda_runtime.h>

#include "DeviceSeq.cuh"

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


////////////////////////////////////////////////

__device__ 
inline void atomicFloatAdd(float *address, float val)
{
	int i_val = __float_as_int(val);
	int tmp0 = 0;
	int tmp1;
	while( (tmp1 = atomicCAS((int *)address, tmp0, i_val)) != tmp0)
	{
		tmp0 = tmp1;
		i_val = __float_as_int(val + __int_as_float(tmp1));
	}
}


template <int D, int DI>//次元,交点を求める方向
__device__
void countCrossingByDirectionDevice
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
		//グローバルメモリへの加算
		atomicFloatAdd(&(counter[ci]),1.0f);
	}
}

////////////////////////////////////////////////

//DI = 0〜D-1 についてcountCrossing()を実行するためにテンプレートの再帰を使用する
template <int D, int DI>//次元,交点を求める方向
struct countCrossingDeviceTemp
{
	__device__
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
		countCrossingDeviceTemp<D,DI-1>::imple(start,end,interval,startindex,indexcount,counter);
		//DI-1方向について平面との交点を求める
		countCrossingByDirectionDevice<D, DI-1>(start,end,interval,startindex,indexcount,counter);
	}
};

//テンプレートの再帰がループにならずに終了するようにテンプレートの特殊化を行う
template <int D>
struct countCrossingDeviceTemp<D, 1>
{
	__device__
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
		countCrossingByDirectionDevice<D,0>(start,end,interval,startindex,indexcount,counter);
	}
};

template <int D>//次元
__device__
void countCrossingDevice
	(
		const float* const start,		//線分の始点
		const float* const end,			//線分の終点
		const float* const interval,	//区間の幅
		const int* const startindex,		//カウンタのインデックスの開始番号
		const int* const indexcount,	//インデックスの個数
		float* const counter		//区間の通過回数のカウンタ
	)
{
	countCrossingDeviceTemp<D,D>::imple(start,end,interval,startindex,indexcount,counter);
}


template <int D, int LC>//次元, ブロックあたりの線分の個数
__global__
void countCrossingSequenceDeviceKernel
	(
		const float* const vertex,		//頂点の列
		int vertexequencecount,			//頂点の列の長さ
		const float* const interval,	//区間の幅
		const int* const startindex,		//カウンタのインデックスの開始番号
		const int* const indexcount,	//インデックスの個数
		float* const counter		//区間の通過回数のカウンタ
	)
{
	//ブロックインデックス、スレッドインデックスの読み替え
	int tx = threadIdx.x;
	//int txsize = blockDim.x;//==LC
	int bx = blockIdx.x;
	//int bxsize = gridDim.x;
	
	//バンクコンフリクト対策のパディング
	const int padding = (D + 1) & 1;
	const int Dp = D + padding;
	
	//シェアードメモリの定義
	__shared__ float vertex_s[(LC +1) * Dp];
	__shared__ float interval_s[D];
	__shared__ int startindex_s[D];
	__shared__ int indexcount_s[D];

	//シェアードメモリへのコピー
	//vertex->vertex_sのコピー
	int vertex_start = bx * LC * Dp;//開始のインデックス
	for(int i = 0; i < Dp; i++)
	{
		int v = LC * i + tx;
		vertex_s[v] = vertex[vertex_start + v];
	}
	if(tx < Dp)
	{
		int v = LC * Dp + tx;
		vertex_s[v] = vertex[vertex_start + v];
	}
	//残りのシェアードメモリのコピー
	if(tx < D)
	{
		interval_s[tx]   = interval[tx];
		startindex_s[tx] = startindex[tx];
		indexcount_s[tx] = indexcount[tx];
	}
	//シェアードメモリへのコピーが完了するまで待機
	__syncthreads();
	
	//線分の始点、終点を定める
	float* start = &vertex_s[ tx      * Dp];
	float* end   = &vertex_s[(tx + 1) * Dp];
	
	//交点の集計処理の実行
	countCrossingDevice<D>
		(
			start,
			end,
			interval_s,
			startindex_s,
			indexcount_s,
			counter
		);

}
template <int D, int LC>//次元, ブロックあたりの線分の個数
__host__
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
	//バンクコンフリクト対策のパディング
	//Dが偶数->1
	//Dが機数->0
	int padding = (D + 1) & 1;
	int Dp = D + padding;
	
	//生成するブロック数
	int blockpadding = ((vertexequencecount - Dp) % (LC * Dp) == 0 ) ? 0 : 1;
	int blockcount = (vertexequencecount - Dp) / (LC * Dp) + blockpadding;
	//デバイスメモリを確保
	//頂点配列
	float* vertex_d;
	int vertex_d_count = (blockcount * LC + 1) * Dp;
	cudaMalloc((void**)&vertex_d, vertex_d_count * sizeof(float));
	cudaMemcpy(vertex_d, vertex, vertexequencecount * sizeof(float), cudaMemcpyHostToDevice);
	std::vector<float> vertex_rem(vertex_d_count - vertexequencecount,-1.0f);
	cudaMemcpy(vertex_d + vertexequencecount, vertex_rem.data(), vertex_rem.size() * sizeof(float), cudaMemcpyHostToDevice);
	//区間の幅
	DeviceSeqConst<float> interval_d(interval,D);
	interval_d.memcpyHostToDevice();
	//カウンタのインデックスの開始番号
	DeviceSeqConst<int> startindex_d(startindex, D);
	startindex_d.memcpyHostToDevice();
	//インデックスの個数
	DeviceSeqConst<int> indexcount_d(indexcount, D);
	indexcount_d.memcpyHostToDevice();
	//区間の通過回数のカウンタ
	int countercount = 1;
	//セル数
	for(int d = 0; d < D; d++)
	{
		countercount *= indexcount[d];
	}
	//セルあたりのカウンタ数
	countercount *= 2 * D;
	DeviceSeq<float> counter_d(counter, countercount);
	counter_d.memcpyHostToDevice();


	
	//カーネルの起動
	countCrossingSequenceDeviceKernel<D,LC><<<blockcount, LC>>>
		(
			vertex_d,
			vertex_d_count,
			interval_d.getDeviceAddress(),
			startindex_d.getDeviceAddress(),
			indexcount_d.getDeviceAddress(),
			counter_d.getDeviceAddress()
		);
	
	//カーネルの結果をデバイスからコピー
	counter_d.memcpyDeviceToHost();

	//デバイスメモリの開放
	cudaFree(vertex_d);
}

#endif
