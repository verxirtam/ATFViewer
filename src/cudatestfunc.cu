/*
 * =====================================================================================
 *
 *       Filename:  cudatestfunc.cu
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年05月17日 00時26分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include <cuda_runtime.h>





__global__ void test0(float* d_a)
{
	int i = threadIdx.x;
	d_a[i] *= 2.0f;
}



void cudatestfunc(float* h_a, int n)
{
	float* d_a;
	cudaMalloc((void**)&d_a, n * sizeof(float));
	cudaMemcpy(d_a, h_a, n * sizeof(float), cudaMemcpyHostToDevice);
	test0<<<1,n>>>(d_a);
	cudaMemcpy(h_a, d_a, n * sizeof(float), cudaMemcpyDeviceToHost);
	cudaFree(d_a);
}
/*
__host__ __device__ int getIndex(float x, float interval)
{
	return (int)floorf(x / interval);
}

__host__ __device__ int* getIndex(const float* const x, const float* const interval, int* const xi)
{
	xi[0] = getIndex(x[0], interval[0]);
	xi[1] = getIndex(x[1], interval[1]);
	xi[2] = getIndex(x[2], interval[2]);
	xi[3] = getIndex(x[3], interval[3]);

	return xi;
}


__host__ __device__ int sign(int x)
{
	return (x > 0) ? 1 : ( (x < 0) ? -1 : 0 );
}

__host__ __device__ int* getDirection(const int* const from, const int* const to, int* const direction)
{
	direction[0] = sign(to[0] - from[0]);
	direction[1] = sign(to[1] - from[1]);
	direction[2] = sign(to[2] - from[2]);
	direction[3] = sign(to[3] - from[3]);
	return direction;
}


__host__ __device__ bool equals(const int* const x0, const int* const x1)
{
	bool ret = true;
	ret = (x0[0] == x1[0]) ? ret : false;
	ret = (x0[1] == x1[1]) ? ret : false;
	ret = (x0[2] == x1[2]) ? ret : false;
	ret = (x0[3] == x1[3]) ? ret : false;
	return ret;
}

//交点を算出する
__host__ __device__ float* getCrossPoint
	(
		const float* const from,//線分の始点
		const float* const to,//線分の終点
		const int* const p_i,//セルの番号
		const float* const interval,//セルの幅
		const int* const dir_i,//線分の方向
		int i,//交点を求める方向
		float* const cross//算出する交点
	)
{
	//交点を求める平面の座標
	float x = interval[i] * (p_i[i] + ((dir_i[i] + 1) / 2));
	//交点の線分のパラメータ
	float s = (x - from[i]) / (to[i] - from[i]);
	//交点の算出
	cross[0] = (to[0] - from[0]) * s + from[0];
	cross[1] = (to[1] - from[1]) * s + from[1];
	cross[2] = (to[2] - from[2]) * s + from[2];
	cross[3] = (to[3] - from[3]) * s + from[3];
	//i成分については誤差が出ないようにもともと求めていたxを使用する
	cross[i] = x;
	//交点を返却
	return cross;
}

//交点が指定したセルの指定した方向の境界にいるかどうか判定する
__host__ __device__ bool atBound
	(
		const float* const cross,//交点
		const int* const p_i,//セル番号
		const float* const interval,//セルの幅
		int i//方向
	)
{
	bool ret = true;
	for(int j = 0; j < 4;j++)
	{
		if(j != i)
		{
			if((cross[i] < interval[j] * p_i[j]) || (interval[j] * (p_i[j] + 1) <= cross[i]))
			{
				ret = false;
			}
		}
		else
		{
			if((cross[i]  < interval[j] * (p_i[j] - 1)) || (interval[j] * (p_i[j] + 2) <= cross[i]))
			{
				ret = false;
			}
		}
	}
	return ret;
}


//線分が通過するセル境界に対し回数をカウントアップする
__host__ __device__ void countCrossing
	(
		const float* const from,//線分の始点
		const float* const to,//線分の終点
		int* const c,//カウンタ
		int n,//カウンタの個数
		const float* const interval//セルの幅
	)
{

	//fromの属するセル番号を取得
	int from_i[4];
	getIndex(from, interval, from_i);
	//toの属するセル番号を取得
	int to_i[4];
	getIndex(to, interval, to_i);
	//線分の各方向の向き
	int direction_i[4];
	getDirection(from_i, to_i, direction_i);
	
	//どのセルにも進まない線分かチェック
	if((direction_i[0] == 0) && (direction_i[1] == 0) && (direction_i[2] == 0) && (direction_i[3] == 0))
	{
		//同じセルにとどまっている線分なので集計なしで終了
		return;
	}

	//ループ用のセル番号
	int p_i[4] = {from_i[0], from_i[1], from_i[2], from_i[3]};
	
	
	//fromからtoまでに通過するセルを求める
	do
	{
		//次に通過するセル
		int next_i[4] = {p_i[0], p_i[1], p_i[2], p_i[3]};
		//p_iの次に通過するセルnext_iを求める
		for(int i = 0; i < 4; i++)
		{
			//next_iが第i方向かどうかチェック
			if(direction_i[i] == 0)
			{
				//この方向には進まないので別の方向をチェック
				continue;
			}
			//toの位置に達していたらこれ以上は進まないので別の方向をチェック
			if(p_i[i] == to_i[i])
			{
				//この方向には進まないので別の方向をチェック
				continue;
			}
			//i方向と垂直な平面との交点を求める
			float cross[4];
			getCrossPoint(from, to, p_i, interval, direction_i, i, cross);
			//現在のセルp_iの境界上の点であればnext_iを確定してbreakでforを抜ける
			if(atBound(cross, p_i, interval, i))
			{
				next_i[i] += direction_i[i];
				break;
			}
		}
		
		//カウントアップを行う
		//対象のカウンタのインデックス
		int c_i = 0;//TODO カウンタのインデックスを算出する関数を作成する
		for(int i = 0; i < 4; i++)
		{
			int c_dir_i = next_i[i] - p_i[i];
			if(c_dir_i != 0)
			{
				c_i += 2 * i + ((c_dir_i > 0) ? 0 : 1);//向きに応じたカウンタのインデックス
				break;
			}
		}
		atomicAdd(&(c[c_i]),1);//カウンタにatomicに加算する <- __host__向けには使えない！！//TODO 修正する！
		
		//p_iをnext_iに更新
		p_i[0] = next_i[0];
		p_i[1] = next_i[1];
		p_i[2] = next_i[2];
		p_i[3] = next_i[3];
	}
	while(!equals(p_i, to_i));//toのセルに到達したら終了
}

 */
