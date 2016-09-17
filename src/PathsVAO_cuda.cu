
#include "PathsVAO.h"
#include "PathsVAO_cuda_kernel.cuh"


void PathsVAO::updateDeviceData(time_t now)
{
	
	////////////////////////////////////////
	// テスト
	////////////////////////////////////////
	//GLの描画が完了するまで待機
	glFinish();
	////////////////////////////////////////
	
	vaoType& vao = doubleBufferingCurrent->vao;
	DeviceMemory<unsigned int>& indexListDevice = doubleBufferingCurrent->indexListDevice;
	
	//VAOをCUDA向けに確保
	Map<vaoType> m(vao);
	
	//VAOのデバイスメモリを取得
	float* v_d = vao.getVertexDevicePointer();
	unsigned int* e_d = vao.getElementDevicePointer();
	
	//indexListDeviceのデバイスメモリを取得
	unsigned int* il_d = indexListDevice.getDevicePointer();
	
	//パスの個数
	unsigned int path_count = ( indexListDevice.getCount() / 4 ) - 1;
	
	//std::cout << "path_count = " << path_count << std::endl;
	
	
	////////////////////////////////////////
	// テスト
	////////////////////////////////////////
	static bool output = true;
	std::vector<unsigned int> il_h_before;
	std::vector<unsigned int> e_h_before;
	if(output)
	{
		output = false;
		//indexListの取得
		std::vector<unsigned int> il_h(indexListDevice.getCount(),0);
		indexListDevice.memcpyDeviceToHost(il_h.data());
		il_h_before = il_h;
		
		//elementの取得
		std::vector<unsigned int> e_h(vao.getElementCount(), 0);
		cudaMemcpy(e_h.data(), e_d, e_h.size() * sizeof(float), cudaMemcpyDeviceToHost);
		e_h_before = e_h;
	}
	
	////////////////////////////////////////
	
	TimeSeparation::Position position = TimeSeparation::Position::current;
	
	time_t time_start = 0;
	time_t time_end   = 0;
	
	//時間の区間の開始・終了時刻を取得する
	this->timeSeparation.getIntervalStatus(position, time_start, time_end);
	
	//DBから取得した時刻の範囲を取得する
	time_start -= this->drawTimeWidth;
	//time_end   += this->drawTimeWidth;
	
	float  now_float = static_cast<float>(now - time_start);
	float past_float = static_cast<float>(now - this->drawTimeWidth - time_start);
	
	//ここにCUDA関数を書く予定
	dim3 grid(path_count, 1, 1);
	dim3 block(1,1,1);
	PathsVAO_updateDeviceDataCUDA<<<grid, block>>>
		(
			now_float,
			past_float,
			v_d,
			e_d,
			il_d,
			path_count
		);
	//CUDA関数が完了するまで待機
	cudaThreadSynchronize();
	
	////////////////////////////////////////
	// テスト
	////////////////////////////////////////
	static bool output1 = true;
	if(output1)
	{
		output1 = false;
		//indexListの取得
		std::vector<unsigned int> il_h(indexListDevice.getCount(),0);
		indexListDevice.memcpyDeviceToHost(il_h.data());
		
		int imax = indexListDevice.getCount() / 4;
		std::cout << "////////////////////////////////////////" << std::endl;
		std::cout << "indexList: " << imax << std::endl;
		for(int i = 0; i < imax; i++)
		{
			bool equal = true;
			std::cout << i << ": ";
			for(int j = 0; j < 4; j++)
			{
				std::cout << il_h_before[i * 4 + j] << " ";
			}
			std::cout << " | ";
			for(int j = 0; j < 4; j++)
			{
				std::cout << il_h[i * 4 + j] << " ";
				
				if(il_h_before[i * 4 + j] != il_h[i * 4 + j])
				{
					equal = false;
				}
			}
			std::cout << " | ";
			if(!equal)
			{
				std::cout << " NOT EQUAL ";
			}
			std::cout << std::endl;
		}
		std::cout << "////////////////////////////////////////" << std::endl;
		
		//elementの取得
		std::vector<unsigned int> e_h(vao.getElementCount(), 0);
		cudaMemcpy(e_h.data(), e_d, e_h.size() * sizeof(float), cudaMemcpyDeviceToHost);
		int kmax = imax - 1;
		for(int k = 0; k < kmax; k++)
		{
			bool equal = true;
			
			std::cout << k << ": ";
			int lmin = il_h[ k      * 4 + 3];
			int lmax = il_h[(k + 1) * 4 + 3];
			for(int l = lmin; l < lmax; l++)
			{
				std::cout << e_h_before[l] << ", ";
			}
			std::cout << std::endl;
			
			std::cout << k << ": ";
			lmin = il_h[ k      * 4 + 3];
			lmax = il_h[(k + 1) * 4 + 3];
			for(int l = lmin; l < lmax; l++)
			{
				std::cout << e_h[l] << ", ";
				
				if(e_h_before[l] != e_h[l])
				{
					equal = false;
				}
			}
			std::cout << std::endl;
			std::cout << k << ": ";
			if(!equal)
			{
				std::cout << "NOT EQUAL";
			}
			std::cout << std::endl;
		}
	}
	
	////////////////////////////////////////
}



