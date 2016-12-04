/*
 * =====================================================================================
 *
 *       Filename:  StringVAO.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年11月28日 01時30分44秒
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
#include <glm/gtc/matrix_transform.hpp>

#include "StringTextureManager.h"
#include "VAOPositionTexture.h"
#include "ShaderProgramTexture.h"

//単一のモデル座標系にある
//複数の文字列を任意の位置に表示する
class StringVAO
{
private:
	//シェーダプログラム
	ShaderProgramTexture shaderProgram;
	//VAOクラス
	using vaoType = VAOPositionTexture<ShaderProgramTexture>;
	//VAOオブジェクト
	vaoType vao;
	//文字コードから文字テクスチャのテクスチャ座標を取得する
	void getTextureCoordinate(int char_code, float& umin, float& vmin, float& umax, float& vmax)
	{
		//範囲外の文字コードは32=' '(スペース)とみなす
		if((char_code < 32) || (char_code >= 160) )
		{
			char_code = 32;
		}
		//文字コードからインデックスを取得
		int n = char_code - 32;
		//文字テクスチャの左上のx座標
		int i = (n % 16) * 32;
		//文字テクスチャの左上のy座標
		int j = (n / 16) * 64;
		//テクスチャ座標の取得
		//0.0f〜1.0fに正規化されているのでテクスチャのサイズで割っている
		umin = static_cast<float>(i     ) / 512.0f;
		vmin = static_cast<float>(j     ) / 512.0f;
		umax = static_cast<float>(i + 32) / 512.0f;
		vmax = static_cast<float>(j + 64) / 512.0f;
		
	}
	//1つの文字列からVAOオブジェクトを生成するための情報を取得する
	//disp_string	表示する文字列
	//transform		文字列を配置する位置を示す変換行列
	//input			文字列から取得した頂点座標・テクスチャ座標を格納するオブジェクト
	//element		インデックス配列を格納するvector
	void setString
		(
			const std::string& disp_string,
			const glm::mat4& transform,
			vaoType::inputType& input,
			std::vector<unsigned int>& element
		);
	//文字の配置から座標変換行列を取得する
	//position		1文字目の左下の点の位置を示す
	//horizontal	1文字目の左下から右下へのベクトル
	//vertical		1文字目の左下から左上へのベクトル
	glm::mat4 getTransform(const glm::vec3& position, const glm::vec3& horizontal, const glm::vec3& vertical);
public:
	//単一の文字列と
	//その配置を示すベクトルを格納する
	struct SingleString
	{
		//表示する文字列
		std::string dispString;
		//1文字目の左下の点の位置を示すベクトル
		glm::vec3   position;
		//1文字目の左下から右下へのベクトル
		glm::vec3   horizontal;
		//1文字目の左下から左上へのベクトル
		glm::vec3   vertical;
		SingleString():
			dispString(),
			position(),
			horizontal(),
			vertical()
		{
		}
	};
	StringVAO():
		shaderProgram(),
		vao(shaderProgram, StringTextureManager::getInstance().getTexture())
	{
	}
	//検証用のコンストラクタ
	//テスト用の文字列一つを初期化する
	void init();
	//stringの内容に従って初期化する
	void init(const std::vector<SingleString>& string);
	//このオブジェクトに属するStringが従うMVPMatrixを設定する
	void setMVPMatrix(const glm::mat4& m)
	{
		shaderProgram.setMVPMatrix(m);
	}
	//描画を行う
	void display()
	{
		vao.display();
	}
};


