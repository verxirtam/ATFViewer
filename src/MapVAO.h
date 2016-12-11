/*
 * =====================================================================================
 *
 *       Filename:  MapVAO.h
 *
 *    Description:  マップを描画する
 *
 *        Version:  1.0
 *        Created:  2016年02月07日 17時23分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once


#include <cstdio>
#include <cstdlib>

#include <sstream>
#include <vector>

#include "OpenGLHeaders.h"

#include "Util.h"
#include <DBAccessor.h>

#include "VAOPositionTexture.h"
#include "ShaderProgramTexture.h"
#include "Texture2D.h"




class MapVAO
{
private:
	struct MapVertex
	{
		float longitude;
		float latitude;
		float u;
		float v;
		MapVertex()
			:
				longitude(0.0f),
				latitude(0.0f),
				u(0.0f),
				v(0.0f)
		{
		}
	};
	
	struct MapVertexIndex
	{
		int vertexIndex;
		MapVertexIndex()
			:
				vertexIndex(0)
		{
		}
	};
	struct InitMapSetting
	{
		std::string textureFileName;
		int textureWidth;
		int textureHeight;
		std::vector<MapVertex> mapVertex;
		std::vector<std::vector<MapVertexIndex> > mapVertexIndex;
		InitMapSetting()
			:
				textureFileName(),
				textureWidth(0),
				textureHeight(0),
				mapVertex(),
				mapVertexIndex()
		{
		}
	};
	std::string mapId;
	Texture2D texture;
	using vaoType = VAOPositionTexture<ShaderProgramTexture>;
	vaoType vao;
	void getSettings(DBAccessor& dba, std::string& map_id, InitMapSetting& ims);
	void getVertex(DBAccessor& dba,std::string& map_id, InitMapSetting& ims);
	void getVertexIndex(DBAccessor& dba,std::string& map_id, InitMapSetting& ims);
	void initVAO(InitMapSetting& ims);
public:
	MapVAO(const std::string& map_id,ShaderProgramTexture& s):
		mapId(map_id),
		texture(GL_TEXTURE0),
		vao(s,texture)
	{
	}
	void init(DBAccessor& dba);
	void display(void);
};





