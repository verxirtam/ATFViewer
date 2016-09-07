
#include "PathsVAO.h"
#include "PathsVAO_cuda_kernel.cuh"


void PathsVAO::updateDeviceData(time_t now)
{
	vaoType& vao = doubleBufferingCurrent->vao;
	DeviceMemory<unsigned int>& indexListDevice = doubleBufferingCurrent->indexListDevice;
	
	//VAOをCUDA向けに確保
	Map<vaoType> m(doubleBufferingCurrent->vao);
	
	//VAOのデバイスメモリを取得
	float* v_d = vao.getVertexDevicePointer();
	unsigned int* e_d = vao.getElementDevicePointer();
	
	//indexListDeviceのデバイスメモリを取得
	unsigned int* il_d = indexListDevice.getDevicePointer();
	
	//パスの個数
	unsigned int path_count = ( indexListDevice.getCount() / 3 ) - 1;
	
	//ここにCUDA関数を書く予定(多分引数足りない)
	dim3 grid(path_count, 1, 1);
	dim3 block(1,1,1);
	PathsVAO_updateDeviceDataCUDA<<<grid, block>>>
		(
			static_cast<float>(now),
			static_cast<float>(now - this->drawTimeWidth),
			v_d,
			e_d,
			il_d,
			path_count
		);
	
}



