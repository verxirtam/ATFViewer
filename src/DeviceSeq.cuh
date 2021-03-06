/*
 * =====================================================================================
 *
 *       Filename:  DeviceSeq.cuh
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月31日 17時03分07秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include <cuda_runtime.h>

/**
 * @brief ホスト上の配列(変更不可)に対応したデバイス上のメモリを確保・使用する
 *
 * ホストにデータを戻さない場合はこれを使用する
 *
 * @tparam T 格納するデータ型
 * @param count 要素数
 * @param host ホスト上の配列
 * @param device デバイス上の配列
 */
template <typename T>
class DeviceSeqConst
{
private:
	int count;
	const T* const host;
	T* device;
public:
	DeviceSeqConst(const T* const h, int c):count(c),host(h),device(NULL)
	{
		cudaMalloc((void**)&device, count * sizeof(T));
	}
	~DeviceSeqConst()
	{
		cudaFree(device);
	}
	void memcpyHostToDevice(void)
	{
		cudaMemcpy(device, host, count * sizeof(T), cudaMemcpyHostToDevice);
	}
	int getCount()
	{
		return count;
	}
	T* getDeviceAddress()
	{
		return device;
	}
};

/**
 * @brief ホスト上の配列(変更可能)に対応したデバイス上のメモリを確保・使用する
 *
 * ホストにデータを戻す場合はこれを使用する
 *
 * @tparam T 格納するデータ型
 * @param dsc DeviceSeqConstを移譲している
 * @param host 変更するためのホスト上の配列
 */
template <typename T>
class DeviceSeq
{
private:
	DeviceSeqConst<T> dsc;
	T* const host;
public:
	DeviceSeq(T* const h, int c):dsc(h,c),host(h)
	{
	}
	~DeviceSeq()
	{
		
	}
	void memcpyHostToDevice(void)
	{
		dsc.memcpyHostToDevice();
	}
	void memcpyDeviceToHost(void)
	{
		cudaMemcpy(host, dsc.getDeviceAddress(), dsc.getCount() * sizeof(T), cudaMemcpyDeviceToHost);
	}
	T* getDeviceAddress()
	{
		return dsc.getDeviceAddress();
	}

};

template <typename T>
class DeviceMemory
{
private:
	T* device;
	unsigned int count;
	
	void free()
	{
		cudaFree(device);
		device = nullptr;
	}
	//ポインタメンバを持つがコピー不要なので禁止する
	//コピーコンストラクタ
	DeviceMemory(const DeviceMemory&) = delete;
	//コピー代入演算子
	DeviceMemory& operator=(const DeviceMemory&) = delete;
	//ムーブコンストラクタ
	DeviceMemory(DeviceMemory&&) = delete;
	//ムーブ代入演算子
	DeviceMemory& operator=(DeviceMemory&&) = delete;

public:
	DeviceMemory():device(nullptr),count(0)
	{
	}
	~DeviceMemory()
	{
		free();
	}
	void malloc(unsigned int c)
	{
		free();
		count = c;
		cudaMalloc((void**)&device, count * sizeof(T));
	}
	void memcpyHostToDevice(const T* const host)
	{
		cudaMemcpy(device, host, count * sizeof(T), cudaMemcpyHostToDevice);
	}
	void memcpyDeviceToHost(T* const host)
	{
		cudaMemcpy(host, device, count * sizeof(T), cudaMemcpyDeviceToHost);
	}
	unsigned int getCount() const
	{
		return count;
	}
	T* getDevicePointer()
	{
		return device;
	}
};












