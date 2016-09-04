
#include "PathsVAO.h"


__global__
void updateDeviceDataCUDA(float* vertex_d, unsigned int* index_list_d, unsigned int path_count)
{
	
}


void PathsVAO::updateDeviceData(time_t now)
{
	//VAOをCUDA向けに確保
	Map<vaoType> m(*vaoCurrent);
	//VAOのデバイスメモリを取得
	float* v_d = vaoCurrent->getVertexDevicePointer();
	//indexListDeviceのデバイスメモリを取得
	unsigned int* il_d = indexListDevice.getDevicePointer();
	
	//パスの個数
	unsigned int path_count = ( indexList.size() / 3 ) - 1;
	
	//ここにCUDA関数を書く予定(多分引数足りない)
	updateDeviceDataCUDA<<<1,1>>>(v_d, il_d, path_count);
}



