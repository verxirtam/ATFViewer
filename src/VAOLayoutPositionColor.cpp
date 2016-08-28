/*
 * =====================================================================================
 *
 *       Filename:  VAOLayoutPositionColor.cpp
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
#include "VAOLayoutPositionColor.h"

void VAOLayoutPositionColor::initData(const inputType& input, std::vector<float>& data)
{
	data.clear();
	
	const std::vector<float>& p = input.position;
	const std::vector<float>& c = input.color;
	
	//頂点数
	unsigned int imax = std::min(p.size(), c.size()) / 3;
	//初期化用のvectorに詰め替える
	for(unsigned int i = 0; i < imax; i++)
	{
		unsigned int i3 = i * 3;
		data.push_back(p[i3    ]);
		data.push_back(p[i3 + 1]);
		data.push_back(p[i3 + 2]);
		data.push_back(c[i3    ]);
		data.push_back(c[i3 + 1]);
		data.push_back(c[i3 + 2]);
	}

}



void VAOLayoutPositionColor::enableVertexAttribPointer()
{
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

