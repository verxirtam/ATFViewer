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

#include <cuda.h>

using namespace std;


#define MIN (0)
#define MAX (20)

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

__global__ void getIndex_02Coalesing(float* w, int* r)
{
	//スレッドインデックス、ブロックインデックスの読み替え
	//（可読性のため）
	int p = threadIdx.x;
	int x = blockIdx.x;
	int y = blockIdx.y;
	int L = blockDim.x;
	int M = gridDim.x;
	
	
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
	
	//スレッドが参照するシェアードメモリのインデックス
	//32bit*3おきなのでバンクコンフリクトは起きない
	int i_s_start = 3 * p;
	int i_s_end   = 3 * p + 1;
	
	//シェアードメモリの参照
	int j_start = (int)floorf(w_s[i_s_start]);
	int j_end   = (int)floorf(w_s[i_s_end  ]);
	
	countCrossing(r, j_start, j_end);
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
	dim3 blocks(2,2,1);
	dim3 threads(33,1,1);
	//dim3 blocks(512,N/512/512,1);
	//dim3 threads(512,1,1);
	
	const int N = threads.x * blocks.x * blocks.y;
	//const int N = 512 * 512 * 32;
	const int RSIZE = MAX - MIN;
	
	
	HostDeviceSeq<float> w(3 * N);
	HostDeviceSeq<int> r01(RSIZE);
	HostDeviceSeq<int> r02(RSIZE);
	
	
	
	srand((unsigned int)time(NULL));
	
	for(int i = 0; i < N; i++)
	{
		w[3 * i    ] = ((float)rand()) / ((float)(RAND_MAX) + 1.0f) * 10.0f;
		w[3 * i + 1] = ((float)rand()) / ((float)(RAND_MAX) + 1.0f) * 10.0f;
		w[3 * i + 2] = 0.0f;//パディング用
	}
	
	for(int i = 0;i < RSIZE; i++)
	{
		r01[i] = 0;
		r02[i] = 0;
	}

	
	
	//ダミーのカーネル関数
	dummy<<< blocks, threads >>>();
	
	clock_t start01 = clock();
	w.memcpyHostToDevice();
	r01.memcpyHostToDevice();
	getIndex_01Simple<<< blocks, threads >>>(w.getDeviceAddress(), r01.getDeviceAddress());
	r01.memcpyDeviceToHost();
	clock_t end01 = clock();
	
	clock_t start02 = clock();
	w.memcpyHostToDevice();
	r02.memcpyHostToDevice();
	int size = 3 * threads.x * sizeof(float);
	getIndex_02Coalesing<<< blocks, threads, size >>>(w.getDeviceAddress(), r02.getDeviceAddress());
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
	cout << "r:\t";
	for(int i = 0;i < RSIZE; i++)
	{
		cout << r01[i] << "\t";
	}
	cout << endl;
	
	cout << "getIndex_01Simple()経過時間 = " << (double)(end01 - start01) / CLOCKS_PER_SEC << "sec." << endl;
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
	
	return 0;
}

