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
