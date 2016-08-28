/*
 * =====================================================================================
 *
 *       Filename:  VAOLayoutPositionColor.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月28日 20時58分33秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include <vector>

#include "OpenGLHeaders.h"

/**
 * @brief Paths用のVAOレイアウト
 *
 * 頂点毎のレイアウトは下記の通り
 * No|Name|Type |Remark
 * --|----|-----|------
 * 1 |x   |float|頂点のx座標
 * 2 |y   |float|頂点のy座標
 * 3 |z   |float|頂点のz座標
 * 4 |t   |float|時刻
 * 5 |r   |float|頂点色(R)
 * 6 |g   |float|頂点色(G)
 * 7 |b   |float|頂点色(B)
 * 8 |a   |float|頂点色(A)
 * 9 |p   |float|バンクコンフリクト対策のパディング
 * 
 * 頂点は航空機毎に、
 * past,nowの後、
 * 時刻の昇順に並ぶ。
 * 同一時刻の頂点はzの降順に並ぶ。
 */
class VAOLayoutPaths
{
private:
	
	struct inputType1
	{
		std::vector<float> position;
		std::vector<float> time;
		std::vector<float> color;
		inputType1():position(),time(),color()
		{
		}
	};
	
	struct inputType2Vertex
	{
		glm::vec3 position;
		float time;
		glm::vec4 color;
		inputType2Vertex():position(),time(),color()
		{
		}
	};
	using inputType2 = std::vector<inputType2Vertex>;
public:
	using inputType = inputType2;
	void initData(const inputType& input, std::vector<float>& data);
	void enableVertexAttribPointer();
};


