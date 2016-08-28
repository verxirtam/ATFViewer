/*
 * =====================================================================================
 *
 *       Filename:  VAOPositionBase.h
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




template <typename S, typename VBOClass, typename VBOElementClass, typename VAOLayoutClass>
class VAOPositionBase
{
private:
	VAOBase base;
	VAOLayoutClass vaoLayout;
	VBOClass vertex;
	VBOElementClass element;
	std::vector<float> vertexData;
	std::vector<unsigned int> elementData;
	GLenum mode;
	int vertexCount;
	S& shaderProgram;
public:
	using inputType = typename VAOLayoutClass::inputType;
	
	VAOPositionBase(S& s)
		:
			base(),
			vaoLayout(),
			vertex(),
			element(),
			vertexData(),
			elementData(),
			mode(0),
			vertexCount(0),
			shaderProgram(s)
	{
	}
	~VAOPositionBase()
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
	void initReady(const inputType& input, const std::vector<unsigned int>& e, GLenum m);
	void initMain();
	void init(const inputType& input, const std::vector<unsigned int>& e, GLenum m);
	void display(void)
	{
		//シェーダを使用開始
		Use<S> s(shaderProgram);
		//自身のVAOをバインド
		Bind<VAOPositionBase<S, VBOClass, VBOElementClass, VAOLayoutClass> > b(*this);
		//インデックス配列をバインド
		Bind<VBOElementClass> be(element);
		
		glDrawElements(mode, vertexCount, GL_UNSIGNED_INT, 0);
		
	}
	int getVertexCount()
	{
		return vertexCount;
	}
	VBOClass& getVertex()
	{
		return vertex;
	}
	VBOElementClass& getElement()
	{
		return element;
	}
};



template <typename S, typename VBOClass, typename VBOElementClass, typename VAOLayoutClass>
void VAOPositionBase<S, VBOClass, VBOElementClass, VAOLayoutClass>::initReady
	(
		const inputType& input,
		const std::vector<unsigned int>& e,
		GLenum m
	)
{
	//引数をメンバ変数に格納
	vaoLayout.initData(/*p,c*/input, vertexData);
	elementData = e;
	
	//modeの設定
	mode = m;
	
	//頂点数を格納
	vertexCount = e.size();
	
}

template <typename S, typename VBOClass, typename VBOElementClass, typename VAOLayoutClass>
void VAOPositionBase<S, VBOClass, VBOElementClass, VAOLayoutClass>::init
	(
		const inputType& input,
		const std::vector<unsigned int>& e,
		GLenum m
	)
{
	initReady(/*p, c,*/input, e, m);
	initMain();
}
template <typename S, typename VBOClass, typename VBOElementClass, typename VAOLayoutClass>
void VAOPositionBase<S, VBOClass, VBOElementClass, VAOLayoutClass>::initMain()
{
	//ベースクラスの初期化
	base.init();
	
	//初期化
	vertex.init(vertexData);
	element.init(elementData);
	
	//自身のVAOをバインド
	Bind<VAOPositionBase<S, VBOClass, VBOElementClass, VAOLayoutClass> > b(*this);
	
	//vertexの初期化
	{
		//頂点配列をバインド
		Bind<VBOClass> bp(vertex);
		
		//インターリーブ配列への属性の割り当て
		vaoLayout.enableVertexAttribPointer();
	}
	//データ格納用のvectorのクリア
	vertexData.clear();
	elementData.clear();
}



