/*
 * =====================================================================================
 *
 *       Filename:  VAOLayoutPositionTexture.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月28日 21時14分28秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "VAOLayoutPositionTexture.h"

void VAOLayoutPositionTexture::initData(const inputType& input, std::vector<float>& data)
{
	data.clear();
	
	const std::vector<float>& p = input.position;
	const std::vector<float>& t = input.texture;
	
	//頂点数の算出
	unsigned int imax = std::min(p.size() / 3, t.size() / 2);
	//インターリーブ配列の作成
	for(unsigned int i = 0; i < imax; i++)
	{
		//positionは3つずつ、
		//textureは2つずつ配置する
		unsigned int i3 = i * 3;
		unsigned int i2 = i * 2;
		
		data.push_back(p[i3    ]);
		data.push_back(p[i3 + 1]);
		data.push_back(p[i3 + 2]);
		data.push_back(t[i2    ]);
		data.push_back(t[i2 + 1]);
	}

}



void VAOLayoutPositionTexture::enableVertexAttribPointer()
{
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

