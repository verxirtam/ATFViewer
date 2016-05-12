/*
 * =====================================================================================
 *
 *       Filename:  cudatest.cu
 *
 *    Description:  cudaの練習用
 *
 *        Version:  1.0
 *        Created:  2016年04月23日 23時47分33秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <iostream>
#include <ctime>
#include <cmath>

#include <cuda.h>

using namespace std;


#define MIN (0)
#define MAX (20)
#define RSIZE (20)

__device__ void countCrossing(int* r, int j_start, int j_end)
{
	
	if(j_start != j_end)
	{
		int diff = (j_start < j_end) ? 1 : -1;
		int kmax = (j_start < j_end) ? (j_end - j_start) : (j_start - j_end);
		for(int k = 0; k < kmax; k++)
		{
			int j = j_start + (k + 1) * diff;
			int j_index = j * 2;
			j_index += diff > 0 ? 0 : 1;
			
			if((MIN <= j_index) && (j_index < MAX))
			{
				atomicAdd(&(r[j_index]),1);
			}
		}
	}
}

//ダミーのカーネル関数
__global__ void dummy(void)
{
}


//比較用の何もしないカーネル関数
//データ転送のみ実施
__global__ void getIndex_00DoNothing(float* w, int* r)
{
	//何もしない
}
//floor(w[i])の個数を集計する
//メモリアクセスの工夫はしない
__global__ void getIndex_01Simple(float* w, int* r)
{
	int i_start = (threadIdx.x + (blockIdx.x + blockIdx.y * gridDim.x ) * blockDim.x) * 3;
	int i_end   = i_start + 1;
	
	int j_start = (int)floorf(w[i_start]);
	int j_end   = (int)floorf(w[i_end  ]);
	
	countCrossing(r, j_start, j_end);
}

__device__ void addResult(int* r_t, int s, int t)
{
	int s2 = (s >> 1);
	if(t < RSIZE * s2)
	{
		r_t[t] += r_t[t + RSIZE * s2];
	}
}

__global__ void getIndex_02Coalesing(float* w, int* r_t, int _2_B, int* r)
{
	//スレッドインデックス、ブロックインデックスの読み替え
	//（可読性のため）
	int p = threadIdx.x;//ブロック内のスレッドのインデックス
	int x = blockIdx.x;//ブロックのインデックスのx成分
	int y = blockIdx.y;//ブロックのインデックスのy成分
	int L = blockDim.x;//ブロック1つあたりのx方向のスレッド数
	int M = gridDim.x;//x方向のブロック数
	
	int b = x + y * M;//ブロックの通し番号
	int t = p + b * L;//スレッドの通し番号
	
	//シェアードメモリ
	//グローバル変数のアクセス用
	extern __shared__ float w_s[];
	
	//グローバルメモリからシェアードメモリにコピー
	//コアレスアクセスになるようにスレッドごとに隣接するアドレスを指定する
	//スレッド数の3倍の個数があるので3回に分ける
	int offset = 3 * (x + y * M) * L;
	w_s[p        ] = w[offset + p        ];
	w_s[p +     L] = w[offset + p +     L];
	w_s[p + 2 * L] = w[offset + p + 2 * L];
	
	//ブロック内のスレッドについて同期する
	//上記のシェアードメモリへのコピーが
	//ブロック内の全てのスレッドで完了するまで待機する
	//ブロック内で起動するスレッドがワープのサイズ（GTX260なら32）
	//を超えるとスレッドが同期して実行されることが保証されなくなる
	//逆に言うと同じワープ内のスレッドは常に同期されているので、
	//工夫すれば__syncthreads()を使用した明示的な同期を不要に出来る可能性がある
	__syncthreads();
	
	//スレッドが参照するシェアードメモリのインデックス
	//32bit*3おきなのでバンクコンフリクトは起きない
	int i_s_start = 3 * p;
	int i_s_end   = 3 * p + 1;
	
	//シェアードメモリの参照
	int j_start = (int)floorf(w_s[i_s_start]);
	int j_end   = (int)floorf(w_s[i_s_end  ]);
	
	
	//shared memory r_s alloc
	__shared__ int r_s[RSIZE];
	
	//copy global memory r to shared memory r_s
	//RSIZE <= L であることを前提にする
	//一部だけコピー
	if(p < RSIZE)
	{
		r_s[p] = r[p];
	}
	//シェアードメモリのコピー中に
	//別のワープのスレッドが集計を始めないように同期する
	__syncthreads();

	//シェアードメモリr_sを使用して集計
	//集計時にブロック内でしか衝突は起きない
	countCrossing(r_s, j_start, j_end);
	
	//集計が終わる前にグローバルメモリr_tへのコピーが
	//始まらないように同期する
	__syncthreads();
	
	//copy shared memory r_s to global memory r
	//シェアードメモリから集計用のグローバルメモリr_tへコピー
	if(p < RSIZE)
	{
		r_t[p + b * RSIZE] = r_s[p];
	}
	//グローバルメモリr_tへのコピーが終わる前に
	//集計が始まらないようにブロック間で同期する
	//そのために一旦カーネルを終了する
	
	
	return;
}


//r_t	集計する値の格納された配列
//    	大きさRSIZEのベクトルが_2_B個格納されている
//_2_B	ベクトルの個数(COUNTPERBLOCKで割り切れること)
//r   	集計結果を格納するベクトル（大きさRSIZE * _2_B / COUNTPERBLOCK）
template <typename COUNTPERBLOCK>//ブロックあたりのベクトルの個数
__global__ void getIndex_02Coalesing_02Accumulate(int* r_t, r_t_copy, int _2_B, int* r)
{
	//スレッドインデックス、ブロックインデックスの読み替え
	//（可読性のため）
	int p = threadIdx.x;//ブロック内のスレッドのインデックス
	int x = blockIdx.x;//ブロックのインデックスのx成分
	int y = blockIdx.y;//ブロックのインデックスのy成分
	int L = blockDim.x;//ブロック1つあたりのx方向のスレッド数
	int M = gridDim.x;//x方向のブロック数
	
	int b = x + y * M;//ブロックの通し番号
	int t = p + b * L;//スレッドの通し番号
	
	
	
	int block_size = COUNTPERBLOCK * RSIZE;
	//所属するブロックのシェアードメモリ
	int r_t_b[COUNTPERBLOCK * RSIZE];

	
	//生成するブロック数
	int block_count = _2_B / COUNTPERBLOCK;
	
	//当該ブロックがアクセスする開始インデックス
	int start_index = b * block_size;

	//結果の格納先の開始インデックス
	int result_start_index = b * RSIZE;
	
	//ブロック内での集計を行う
	//r_t + start_index: 開始インデックス
	//r + result_start_index: 結果の格納先の開始アドレス
	//COUNTPERBLOCK: ブロック内のベクトルの個数
	//r_t_b: ブロック内で使用するシェアードメモリ（サイズ：COUNTPERBLOCK * RSIZE）
	getIndex_02Coalesing_02Accumulate_01ReduceAtBlock(r_t + start_index, r + result_start_index, COUNTPERBLOCK, r_t_b);
	
	//ブロック間の同期がいるためここでカーネルを終了する
	
	//このカーネルが終了した時点で各ブロックの集計が完了しr[]に格納されている
	//結果はblock_count個のベクトルなので、この関数をblock_count==1になるまで繰り返して集計結果を得る
}

template <typename T>
class HostDeviceSeq
{
private:
	int count;
	T* host;
	T* device;
public:
	HostDeviceSeq(int c):host(NULL),device(NULL)
	{
		count = c;
		host = new T[count];
		cudaMalloc((void**)&device, count * sizeof(T));
	}
	~HostDeviceSeq()
	{
		cudaFree(device);
		delete [] host;
	}
	T& operator[](int i)
	{
		return host[i];
	}
	void memcpyHostToDevice(void)
	{
		cudaMemcpy(device, host, count * sizeof(T), cudaMemcpyHostToDevice);
	}
	void memcpyDeviceToHost(void)
	{
		cudaMemcpy(host, device, count * sizeof(T), cudaMemcpyDeviceToHost);
	}
	T* getDeviceAddress()
	{
		return device;
	}
};

int main(int argc, char const* argv[])
{
	dim3 blocks(8,2,1);
	//dim3 blocks(128,64,1);
	dim3 threads(512,1,1);
	//dim3 blocks(512,N/512/512,1);
	//dim3 threads(512,1,1);
	
	const int N = threads.x * blocks.x * blocks.y;
	//const int N = 512 * 512 * 32;
	//const int RSIZE = MAX - MIN;
	
	
	HostDeviceSeq<float> w(3 * N);
	HostDeviceSeq<int> r00(RSIZE);
	HostDeviceSeq<int> r01(RSIZE);
	HostDeviceSeq<int> r02(RSIZE);
	
	int B = (int)ceil(log(blocks.x * blocks.y)/log(2.0f));
	int _2_B = (1 << B);//2^B
	int r_t_size = RSIZE * (_2_B);//RSIZE * 2^B
	HostDeviceSeq<int> r_t(r_t_size);
	
	
	srand((unsigned int)time(NULL));
	
	for(int i = 0; i < N; i++)
	{
		w[3 * i    ] = ((float)rand()) / ((float)(RAND_MAX) + 1.0f) * 10.0f;
		w[3 * i + 1] = ((float)rand()) / ((float)(RAND_MAX) + 1.0f) * 10.0f;
		w[3 * i + 2] = 0.0f;//パディング用
	}
	
	for(int i = 0;i < RSIZE; i++)
	{
		r00[i] = 0;
		r01[i] = 0;
		r02[i] = 0;
	}

	for(int i = 0; i < r_t_size; i++)
	{
		r_t[i]=0;
	}
	
	
	
	
	
	
	//ダミーのカーネル関数
	dummy<<< 1, 1 >>>();
	
	//デバイスへのデータのコピー
	clock_t start_cpy = clock();
	w.memcpyHostToDevice();
	clock_t end_cpy = clock();
	
	clock_t start00 = clock();
	r01.memcpyHostToDevice();
	getIndex_00DoNothing<<< blocks, threads >>>(w.getDeviceAddress(), r00.getDeviceAddress());
	r01.memcpyDeviceToHost();
	clock_t end00 = clock();
	
	clock_t start01 = clock();
	r01.memcpyHostToDevice();
	getIndex_01Simple<<< blocks, threads >>>(w.getDeviceAddress(), r01.getDeviceAddress());
	r01.memcpyDeviceToHost();
	clock_t end01 = clock();
	
	clock_t start02 = clock();
	r02.memcpyHostToDevice();
	r_t.memcpyHostToDevice();
	int size = 3 * threads.x * sizeof(float);
	getIndex_02Coalesing<<< blocks, threads, size >>>(w.getDeviceAddress(), r_t.getDeviceAddress(), _2_B, r02.getDeviceAddress());
	r02.memcpyDeviceToHost();
	clock_t end02 = clock();
	
	
	
	cout << "N = " << N << endl;
	if(false)
	{
		cout << "w:\t";
		for(int i = 0;i < 3 * N; i++)
		{
			cout << w[i] << "\t";
		}
		cout << endl;
	}
	cout << "r01:\t";
	for(int i = 0;i < RSIZE; i++)
	{
		cout << r01[i] << "\t";
	}
	cout << endl;
	cout << "r02:\t";
	for(int i = 0;i < RSIZE; i++)
	{
		cout << r02[i] << "\t";
	}
	cout << endl;
	
	cout << "w.memcpyHostToDevice()経過時間 = " << (double)(end_cpy - start_cpy) / CLOCKS_PER_SEC << "sec." << endl;
	cout << "getIndex_00DoNothing()経過時間 = " << (double)(end00 - start00) / CLOCKS_PER_SEC << "sec." << endl;
	cout << "getIndex_01Simple()経過時間    = " << (double)(end01 - start01) / CLOCKS_PER_SEC << "sec." << endl;
	cout << "getIndex_02Coalesing()経過時間 = " << (double)(end02 - start02) / CLOCKS_PER_SEC << "sec." << endl;
	
	bool result = true;
	for(int i = 0;i < RSIZE; i++)
	{
		if(r01[i]!=r02[i])
		{
			result = false;
		}
	}
	if(result)
	{
		cout << "getIndex_02Coalesing()の実行に成功しました" << endl;
	}
	else
	{
		cout << "getIndex_02Coalesing()の結果が不正です" << endl;
	}
	
	//テンポラリ領域の内容の確認
	if(true)
	{
		r_t.memcpyDeviceToHost();
		cout << "r_t_size : " << r_t_size << endl;
		cout << "r_t[] : " << endl;
		for(int i = 0; i < _2_B; i++)
		{
			cout << "\t";
			for(int j = 0; j < RSIZE; j++)
			{
				int index = j + i * RSIZE;
				cout << r_t[index] << "\t";
			}
			cout << endl;
		}
		cout << endl;
	}
	return 0;
}

