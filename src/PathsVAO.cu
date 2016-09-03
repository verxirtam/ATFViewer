
#include "PathsVAO.h"




void PathsVAO::updateDeviceData(time_t now)
{
	//VAOをCUDA向けに確保
	Map<vaoType> m(*vaoCurrent);
	//VAOのデバイスメモリを取得
	float* v_d = vaoCurrent->getVertexDevicePointer();
	//indexListDeviceのデバイスメモリを取得
	unsigned int* il_d = indexListDevice.getDevicePointer();
	//ここにCUDA関数を書く予定(多分引数足りない)
	updateDeviceDataCUDA(v_d, il_d);
}



