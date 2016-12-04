/*
 * =====================================================================================
 *
 *       Filename:  StringVAO.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年12月04日 02時05分10秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "StringVAO.h"


void StringVAO::setString
	(
		const std::string& disp_string,
		const glm::mat4& transform,
		vaoType::inputType& input,
		std::vector<unsigned int>& element
	)
{
	//現時点の頂点のインデックスのサイズ
	//vec3なので3で割る
	unsigned int begin = input.position.size() / 3;
	
	//stringの各文字についてループ
	unsigned int nmax = disp_string.length();
	for (unsigned int n = 0; n < nmax; n++)
	{

		//1文字を表示する頂点を作成
		std::vector<glm::vec4> add_position_vec3
		{
			transform * glm::vec4(0.5f *  n     , 1.0f, 0.0f, 1.0f),//begin + 4 * n
			transform * glm::vec4(0.5f *  n     , 0.0f, 0.0f, 1.0f),//begin + 4 * n + 1
			transform * glm::vec4(0.5f * (n + 1), 1.0f, 0.0f, 1.0f),//begin + 4 * n + 2
			transform * glm::vec4(0.5f * (n + 1), 0.0f, 0.0f, 1.0f) //begin + 4 * n + 3
		};
		
		//floatのvectorにならす
		std::vector<float> add_position
		{
			add_position_vec3[0].x, add_position_vec3[0].y, add_position_vec3[0].z,
			add_position_vec3[1].x, add_position_vec3[1].y, add_position_vec3[1].z,
			add_position_vec3[2].x, add_position_vec3[2].y, add_position_vec3[2].z,
			add_position_vec3[3].x, add_position_vec3[3].y, add_position_vec3[3].z
		};
		//既存のinput.positionに追加
		input.position.insert(input.position.end(), add_position.begin(), add_position.end());

		//1文字分のインデックス配列
		std::vector<unsigned int> add_element
		{
			begin + 4 * n,
			begin + 4 * n + 1,
			begin + 4 * n + 2,
			begin + 4 * n + 2,
			begin + 4 * n + 1,
			begin + 4 * n + 3,
		};
		//既存のelementに追加
		element.insert(element.end(), add_element.begin(), add_element.end());

		float u0, v0;
		float u1, v1;

		//disp_string[n]に対応するテクスチャ座標を取得
		getTextureCoordinate(disp_string[n], u0, v0, u1, v1);
		//頂点に対応するテクスチャ座標を設定
		std::vector<float> add_texture
		{
			u0, v0,
			u0, v1,
			u1, v0,
			u1, v1
		};
		//既存のinput.textureに追加
		input.texture.insert(input.texture.end(), add_texture.begin(), add_texture.end());
	}
}


glm::mat4 StringVAO::getTransform(const glm::vec3& position, const glm::vec3& horisontal, const glm::vec3& vertical)
{
	//結果を格納する変換行列
	glm::mat4 transform;
	
	//文字列の水平方向
	transform[0] = glm::vec4(horisontal, 0.0f);
	//文字列の垂直方向
	transform[1] = glm::vec4(vertical  , 0.0f);
	//文字列を描画する面の法線ベクトル
	glm::vec3 cross = glm::cross(horisontal, vertical);
	transform[2] = glm::normalize(glm::vec4(cross, 0.0f));
	
	//平行移動
	transform[3] = glm::vec4(position  , 1.0f);
	
	return transform;
}

void StringVAO::init()
{
	//初期化用の文字列の列
	std::vector<SingleString> string;
	//初期化用の文字列
	SingleString single_string;
	single_string.dispString = std::string("test."),
	single_string.position   = glm::vec3(139.0 + ( 46.0/60.0 + 87.0/3600.0 ), 35.0 + ( 33.0/60.0 + 20.0/3600.0 ), 100.0f);
	single_string.horisontal = glm::vec3(1.0f, 0.0f, 0.0f);
	single_string.vertical   = glm::vec3(0.0f, 1.0f, 0.0f);
	
	//stringに追加
	string.push_back(single_string);
	//初期化
	init(string);
}
void StringVAO::init(const std::vector<SingleString>& string)
{
	//シェーダプログラムの初期化
	shaderProgram.init();
	//MVPMatrixの初期化
	glm::mat4 m;
	m = glm::scale(m, glm::vec3(0.125f, 0.125f, 0.125f));
	shaderProgram.setMVPMatrix(m);
	//文字列の初期化
	vaoType::inputType input;
	std::vector<unsigned int> element;
	
	//文字列の列から1個ずつ文字列を取得して初期化する
	for(auto&& s : string)
	{
		//座標変換行列の取得
		glm::mat4 transform = getTransform(s.position, s.horisontal, s.vertical);
		//1つの文字列分のVAOの情報を取得
		setString(s.dispString, transform, input, element);
	}
	
	//VAOの初期化
	vao.init(input, element, GL_TRIANGLES);
	
}

