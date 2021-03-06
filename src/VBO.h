/*
 * =====================================================================================
 *
 *       Filename:  VBOStatic.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月31日 19時47分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include "OpenGLHeaders.h"

#include <vector>

#include "Lock.h"

#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

template<typename V>
class VBOBase
{
private:
	GLuint handle;
	const GLenum type;
	const GLenum usage;

	//ポインタメンバを持つがコピー不要なので禁止する
	//コピーコンストラクタ
	VBOBase(const VBOBase&) = delete;
	//コピー代入演算子
	VBOBase& operator=(const VBOBase&) = delete;
	//ムーブコンストラクタ
	VBOBase(VBOBase&&) = delete;
	//ムーブ代入演算子
	VBOBase& operator=(VBOBase&&) = delete;
public:
	VBOBase(GLenum t, GLenum u):handle(0),type(t),usage(u)
	{
	}
	~VBOBase()
	{
		GL_CALL(glDeleteBuffers(1, &handle));
	}
	void bind()
	{
		GL_CALL(glBindBuffer(type, handle));
	}
	void unbind()
	{
		GL_CALL(glBindBuffer(type, 0));
	}
	void init(const std::vector<V>& v)
	{
		//handle!=0の時のためにdeleteしておく
		//handleが0の時は無視される
		GL_CALL(glDeleteBuffers(1, &handle));
		//バッファオブジェクトの確保
		GL_CALL(glGenBuffers(1, &handle));
		
		//バッファのバインド
		Bind<VBOBase<V> > b(*this);
		//バッファのサイズ
		GLsizeiptr size = v.size() * sizeof(V);
		//バッファにデータを格納(GPUへ転送)
		GL_CALL(glBufferData(type, size, v.data(), usage));
	}
	GLuint getHandle()
	{
		return handle;
	}
};

//using VBOStatic        = VBOBase<        GL_ARRAY_BUFFER, GL_STATIC_DRAW,        float>;
//using VBOElementStatic = VBOBase<GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, unsigned int>;

class VBOStatic
{
private:
	VBOBase<float> base;
public:
	VBOStatic():base(GL_ARRAY_BUFFER,GL_STATIC_DRAW)
	{
	}
	void bind()
	{
		base.bind();
	}
	void unbind()
	{
		base.unbind();
	}
	void init(const std::vector<float>& v)
	{
		base.init(v);
	}
};

class VBOElementStatic
{
private:
	VBOBase<unsigned int> base;
public:
	VBOElementStatic():base(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW)
	{
	}
	void bind()
	{
		base.bind();
	}
	void unbind()
	{
		base.unbind();
	}
	void init(const std::vector<unsigned int>& v)
	{
		base.init(v);
	}
};


///////////////////////////

//VBODynamicBaseでのVBOのCUDA向けの設定

//VBODYNAMICBASE_STANDARD
//cudaのドキュメントに従った方法
//map(),unmap()でVBOのリソースをCUDA向けに確保する
//ドキュメントに従っているが、
//PCIeの通信量(rx, tx)が増加する(nvidia-smiで確認)
//
//VBODYNAMICBASE_LOWTHROUGHPUT
//cudaのドキュメントに従っていないが
//PCIeの通信量がほとんど増加しない方法
//init()時にリソースの確保とアドレスの取得をしてしまう
//map(),unmap()時は何もしない

//下記2行のうちのどちらか1方のみをコメントアウトして設定する
//#define VBODYNAMICBASE_STANDARD
#define VBODYNAMICBASE_LOWTHROUGHPUT

template <int VboType, typename ElementType>
class VBODynamicBase
{
private:
	VBOBase<ElementType> base;
	cudaGraphicsResource* resource;
	ElementType* device;
	//CUDA向けのグラフィックスリソースを開放する
	void unregister()
	{
		if(resource != nullptr)
		{
			cudaGraphicsUnregisterResource(resource);
		}
	}
	
	//ポインタメンバを持つがコピー不要なので禁止する
	//コピーコンストラクタ
	VBODynamicBase(const VBODynamicBase&) = delete;
	//コピー代入演算子
	VBODynamicBase& operator=(const VBODynamicBase&) = delete;
	//ムーブコンストラクタ
	VBODynamicBase(VBODynamicBase&&) = delete;
	//ムーブ代入演算子
	VBODynamicBase& operator=(VBODynamicBase&&) = delete;
public:
	VBODynamicBase():base(VboType, GL_DYNAMIC_DRAW),resource(nullptr), device(nullptr)
	{
	}
	~VBODynamicBase()
	{
		unregister();
	}
	void init(const std::vector<ElementType>& v)
	{
		base.init(v);
		//CUDA向けのグラフィックスリソースを開放
		unregister();
		//CUDA向けのグラフィックスリソースの取得
		cudaGraphicsGLRegisterBuffer(&resource, base.getHandle(), cudaGraphicsRegisterFlagsNone);
		
		#ifdef VBODYNAMICBASE_LOWTHROUGHPUT
		cudaGraphicsMapResources(1, &resource, nullptr);
		cudaGraphicsResourceGetMappedPointer((void**)&device, nullptr, resource);
		cudaGraphicsUnmapResources(1, &resource, nullptr);
		#endif
	}
	void bind()
	{
		base.bind();
	}
	void unbind()
	{
		base.unbind();
	}
	void map()
	{
		#ifdef VBODYNAMICBASE_STANDARD
		cudaGraphicsMapResources(1, &resource, nullptr);
		cudaGraphicsResourceGetMappedPointer((void**)&device, nullptr, resource);
		#endif
	}
	void unmap()
	{
		#ifdef VBODYNAMICBASE_STANDARD
		cudaGraphicsUnmapResources(1, &resource, nullptr);
		#endif
	}
	ElementType* getDevicePointer()
	{
		return device;
	}
};

///////////////////////////

using VBODynamic        = VBODynamicBase<GL_ARRAY_BUFFER        , float       >;
using VBOElementDynamic = VBODynamicBase<GL_ELEMENT_ARRAY_BUFFER, unsigned int>;



