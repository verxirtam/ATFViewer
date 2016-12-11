/*
 * =====================================================================================
 *
 *       Filename:  VAOLayoutPaths.cpp
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
#include "VAOLayoutPaths.h"

void VAOLayoutPaths::initData(const inputType& input, std::vector<float>& data)
{
	data.clear();
	
	//頂点数
	unsigned int imax = input.size();
	//初期化用のvectorに詰め替える
	for(unsigned int i = 0; i < imax; i++)
	{
		data.push_back(input[i].position.x);//頂点x座標
		data.push_back(input[i].position.y);//頂点y座標
		data.push_back(input[i].position.z);//頂点z座標
		data.push_back(input[i].time);//時刻
		data.push_back(input[i].color.r);//頂点色(R)
		data.push_back(input[i].color.g);//頂点色(G)
		data.push_back(input[i].color.b);//頂点色(B)
		data.push_back(input[i].color.a);//頂点色(アルファ)
		data.push_back(0.0f);//バンクコンフリクト対策のパディング
	}

}



void VAOLayoutPaths::enableVertexAttribPointer()
{
	//position部分の設定
	GL_CALL(glVertexAttribPointer
		(
			0,		//設定するバーテックスシェーダの引数のインデックスを指定する
			3,		//1頂点あたりの要素数(ここでは3次元座標なので3)
			GL_FLOAT,	//要素の型
			GL_FALSE,	//正規化の要否 位置座標なのでFalse
			9 * sizeof(float),		//頂点データ同士の間隔(byte単位) 0なら隙間なく配置されているとみなされる
			(GLfloat*)0	//頂点データの開始アドレスから指定するデータの位置までの間隔
		));
	GL_CALL(glEnableVertexAttribArray(0));
	//color部分の設定
	GL_CALL(glVertexAttribPointer
		(
			1,		//設定するバーテックスシェーダの引数のインデックスを指定する
			4,		//1頂点あたりの要素数(ここではrgbaなので4)
			GL_FLOAT,	//要素の型
			GL_FALSE,	//正規化の要否 位置座標なのでFalse
			9 * sizeof(float),		//頂点データ同士の間隔(byte単位) 0なら隙間なく配置されているとみなされる
			(GLfloat*)0 + 4	//頂点データの開始アドレスから指定するデータの位置までの間隔
		));
	GL_CALL(glEnableVertexAttribArray(1));

}

