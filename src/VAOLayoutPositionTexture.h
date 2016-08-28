/*
 * =====================================================================================
 *
 *       Filename:  VAOLayoutPositionTexture.h
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

class VAOLayoutPositionTexture
{
private:
	struct inputType1
	{
		std::vector<float> position;
		std::vector<float> texture;
		inputType1():position(),texture()
		{
		}
	};
	struct inputType2Vertex
	{
		glm::vec3 position;
		glm::vec2 texture;
		inputType2Vertex():position(),texture()
		{
		}
	};
	using inputType2 = std::vector<inputType2Vertex>;
public:
	using inputType = inputType1;
	void initData(const inputType& input, std::vector<float>& data);
	void enableVertexAttribPointer();
};


