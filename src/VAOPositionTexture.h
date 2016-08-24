/*
 * =====================================================================================
 *
 *       Filename:  VAOPositionTexture.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月08日 03時49分15秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include <GL/glew.h>
#include <GL/glut.h>

#include "Lock.h"
#include "VBO.h"
#include "Texture2D.h"
#include "VAOBase.h"

template <typename S>
class VAOPositionTexture
{
private:
	VAOBase base;
	VBOStatic positionTexture;
	VBOElementStatic element;
	GLenum mode;
	int vertexCount;
	S& shaderProgram;
	Texture2D& texture;
	void initPositionTexture(const std::vector<float>& p, const std::vector<float>& t);
	
public:
	VAOPositionTexture(S& s, Texture2D& t):
		base(),
		positionTexture(),
		element(),
		mode(0),
		vertexCount(0),
		shaderProgram(s),
		texture(t)
	{
	}
	~VAOPositionTexture()
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
	void init
		(
			const std::vector<float>& p,
			const std::vector<float>& c,
			const std::vector<unsigned int>& e,
			GLenum m
		);
	void display(void)
	{
		//シェーダを使用開始
		Use<S> s(shaderProgram);
		//テクスチャのバインド
		Active<Texture2D> ta(texture);
		Bind<Texture2D> tb(texture);
		//自身のVAOをバインド
		Bind<VAOPositionTexture> b(*this);
		//インデックス配列をバインド
		Bind<VBOElementStatic> be(element);
		
		glDrawElements(mode, vertexCount, GL_UNSIGNED_INT, 0);
		
	}
};


template <typename S>
void VAOPositionTexture<S>::initPositionTexture
	(
		const std::vector<float>& p,
		const std::vector<float>& t
	)
{
	//初期化用のインターリーブ配列
	std::vector<float> pt;
	//頂点数の算出
	unsigned int imax = std::min(p.size() / 3, t.size() / 2);
	//インターリーブ配列の作成
	for(unsigned int i = 0; i < imax; i++)
	{
		//positionは3つずつ、
		//textureは2つずつ配置する
		unsigned int i3 = i * 3;
		unsigned int i2 = i * 2;
		
		pt.push_back(p[i3    ]);
		pt.push_back(p[i3 + 1]);
		pt.push_back(p[i3 + 2]);
		pt.push_back(t[i2    ]);
		pt.push_back(t[i2 + 1]);
	}
	//作成したインターリーブ配列を使用して初期化
	positionTexture.init(pt);
}

template <typename S>
void VAOPositionTexture<S>::init
	(
		const std::vector<float>& p,
		const std::vector<float>& t,
		const std::vector<unsigned int>& e,
		GLenum m
	)
{
	base.init();
	
	//引数をバッファに格納
	initPositionTexture(p, t);
	element.init(e);
	//頂点数を格納
	vertexCount = e.size();
	
	//modeの設定
	mode = m;
	
	//自身のVAOをバインド
	Bind<VAOPositionTexture> b(*this);
	//positionTextureの設定
	{
		//頂点配列をバインド
		Bind<VBOStatic> bp(positionTexture);
		//position部分の設定
		glVertexAttribPointer
			(
				0,		//設定するバーテックスシェーダの引数のインデックスを指定する
				3,		//1頂点あたりの要素数(ここでは3次元座標なので3)
				GL_FLOAT,	//要素の型
				GL_FALSE,	//正規化の要否 位置座標なのでFalse
				5 * sizeof(float),		//頂点データ同士の間隔(byte単位) 0なら隙間なく配置されているとみなされる
				(GLfloat*)0	//頂点データの開始アドレスから指定するデータの位置までの間隔
			);
		glEnableVertexAttribArray(0);
		//texture部分の設定
		glVertexAttribPointer
			(
				1,		//設定するバーテックスシェーダの引数のインデックスを指定する
				2,		//1頂点あたりの要素数(ここでは3次元座標なので3)
				GL_FLOAT,	//要素の型
				GL_FALSE,	//正規化の要否 位置座標なのでFalse
				5 * sizeof(float),		//頂点データ同士の間隔(byte単位) 0なら隙間なく配置されているとみなされる
				(GLfloat*)0 + 3	//頂点データの開始アドレスから指定するデータの位置までの間隔
			);
		glEnableVertexAttribArray(1);
	}
	
}

