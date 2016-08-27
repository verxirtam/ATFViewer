/*
 * =====================================================================================
 *
 *       Filename:  VAOPositionColor.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月31日 19時48分15秒
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

#include <algorithm>

#include "Lock.h"
#include "VBO.h"
#include "VAOBase.h"


template <typename S, typename VBOClass, typename VBOElementClass>
class VAOPositionColorBase
{
private:
	VAOBase base;
	VBOClass positionColor;
	VBOElementClass element;
	std::vector<float> positionColorData;
	std::vector<unsigned int> elementData;
	GLenum mode;
	int vertexCount;
	S& shaderProgram;
	void initPositionColor(const std::vector<float>& p, const std::vector<float>& c);
public:
	VAOPositionColorBase(S& s)
		:
			base(),
			positionColor(),
			element(),
			positionColorData(),
			elementData(),
			mode(0),
			vertexCount(0),
			shaderProgram(s)
	{
	}
	~VAOPositionColorBase()
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
	void initReady(const std::vector<float>& p, const std::vector<float>& c, const std::vector<unsigned int>& e, GLenum m);
	void initMain();
	void init(const std::vector<float>& p, const std::vector<float>& c, const std::vector<unsigned int>& e, GLenum m);
	void display(void)
	{
		//シェーダを使用開始
		Use<S> s(shaderProgram);
		//自身のVAOをバインド
		Bind<VAOPositionColorBase<S, VBOClass, VBOElementClass> > b(*this);
		//インデックス配列をバインド
		Bind<VBOElementClass> be(element);
		
		glDrawElements(mode, vertexCount, GL_UNSIGNED_INT, 0);
		
	}
	int getVertexCount()
	{
		return vertexCount;
	}
	VBOElementClass& getElement()
	{
		return element;
	}
};


template <typename S, typename VBOClass, typename VBOElementClass>
void VAOPositionColorBase<S, VBOClass, VBOElementClass>::initPositionColor
	(
		const std::vector<float>& p,
		const std::vector<float>& c
	)
{
	//初期化用のvector
	positionColorData.clear();
	
	//頂点数
	unsigned int imax = std::min(p.size(), c.size()) / 3;
	//初期化用のvectorに詰め替える
	for(unsigned int i = 0; i < imax; i++)
	{
		unsigned int i3 = i * 3;
		positionColorData.push_back(p[i3    ]);
		positionColorData.push_back(p[i3 + 1]);
		positionColorData.push_back(p[i3 + 2]);
		positionColorData.push_back(c[i3    ]);
		positionColorData.push_back(c[i3 + 1]);
		positionColorData.push_back(c[i3 + 2]);
	}
}

template <typename S, typename VBOClass, typename VBOElementClass>
void VAOPositionColorBase<S, VBOClass, VBOElementClass>::initReady
	(
		const std::vector<float>& p,
		const std::vector<float>& c,
		const std::vector<unsigned int>& e,
		GLenum m
	)
{
	//引数をメンバ変数に格納
	initPositionColor(p,c);
	elementData = e;
	
	//modeの設定
	mode = m;
	
	//頂点数を格納
	vertexCount = e.size();
	
}

template <typename S, typename VBOClass, typename VBOElementClass>
void VAOPositionColorBase<S, VBOClass, VBOElementClass>::init
	(
		const std::vector<float>& p,
		const std::vector<float>& c,
		const std::vector<unsigned int>& e,
		GLenum m
	)
{
	initReady(p, c, e, m);
	initMain();
}
template <typename S, typename VBOClass, typename VBOElementClass>
void VAOPositionColorBase<S, VBOClass, VBOElementClass>::initMain()
{
	//ベースクラスの初期化
	base.init();
	
	//初期化
	positionColor.init(positionColorData);
	element.init(elementData);
	
	//自身のVAOをバインド
	Bind<VAOPositionColorBase<S, VBOClass, VBOElementClass> > b(*this);
	
	//positionColorの初期化
	{
		//頂点配列をバインド
		Bind<VBOClass> bp(positionColor);
		//point部分の設定
		glVertexAttribPointer
			(
				0,		//設定するバーテックスシェーダの引数のインデックスを指定する
				3,		//1頂点あたりの要素数(ここでは3次元座標なので3)
				GL_FLOAT,	//要素の型
				GL_FALSE,	//正規化の要否 位置座標なのでFalse
				6 * sizeof(float),		//頂点データ同士の間隔(byte単位) 0なら隙間なく配置されているとみなされる
				(GLfloat*)0	//頂点データの開始アドレスから指定するデータの位置までの間隔
			);
		glEnableVertexAttribArray(0);
		//color部分の設定
		glVertexAttribPointer
			(
				1,		//設定するバーテックスシェーダの引数のインデックスを指定する
				3,		//1頂点あたりの要素数(ここではrgbなので3)
				GL_FLOAT,	//要素の型
				GL_FALSE,	//正規化の要否 位置座標なのでFalse
				6 * sizeof(float),		//頂点データ同士の間隔(byte単位) 0なら隙間なく配置されているとみなされる
				(GLfloat*)0 + 3	//頂点データの開始アドレスから指定するデータの位置までの間隔
			);
		glEnableVertexAttribArray(1);
	}
	//データ格納用のvectorのクリア
	positionColorData.clear();
	elementData.clear();
}

template <typename S>
using VAOPositionColor = VAOPositionColorBase<S, VBOStatic, VBOElementStatic>;

//template <typename S>
//using VAOPositionColorDynamic = VAOPositionColorBase<S, VBODynamic, VBOElementDynamic>;
template <typename S>
class VAOPositionColorDynamic
{
private:
	VAOPositionColorBase<S, VBODynamic, VBOElementDynamic> base;


public:
	VAOPositionColorDynamic(S& s):base(s)
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
	void initReady(const std::vector<float>& p, const std::vector<float>& c, const std::vector<unsigned int>& e, GLenum m)
	{
		base.initReady(p, c, e, m);
	}
	void initMain()
	{
		base.initMain();
	}
	void init(const std::vector<float>& p, const std::vector<float>& c, const std::vector<unsigned int>& e, GLenum m)
	{
		base.init(p, c, e, m);
	}
	void display()
	{
		base.display();
	}
	void map()
	{
		base.getPosition().map();
		base.getColor().map();
		base.getElement().map();
	}
	void unmap()
	{
		base.getPosition().unmap();
		base.getColor().unmap();
		base.getElement().unmap();
	}
	float* getPositionDevicePointer()
	{
		return base.getPosition().getDevicePointer();
	}
	float* getColorDevicePointer()
	{
		return base.getColor().getDevicePointer();
	}
	unsigned int* getElementDevicePointer()
	{
		return base.getElement().getDevicePointer();
	}
	int getVertexCount()
	{
		return base.getVertexCount();
	}
};


