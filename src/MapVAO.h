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


#ifndef MAP_H_
#define MAP_H_

#include <cstdio>
#include <cstdlib>

#include <sstream>
#include <vector>

#include "OpenGLHeaders.h"

#include "Util.h"
#include "DBAccessor.h"

#include "TextureShaderProgram.h"
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
	};
	
	struct MapVertexIndex
	{
		int vertexIndex;
	};
	struct InitMapSetting
	{
		std::string textureFileName;
		int textureWidth;
		int textureHeight;
		std::vector<MapVertex> mapVertex;
		std::vector<std::vector<MapVertexIndex> > mapVertexIndex;
	};
	std::string mapId;
	Texture2D texture;
	TextureShaderProgram::vaoType vao;
	void getSettings(DBAccessor& dba, std::string& map_id, InitMapSetting& ims);
	void getVertex(DBAccessor& dba,std::string& map_id, InitMapSetting& ims);
	void getVertexIndex(DBAccessor& dba,std::string& map_id, InitMapSetting& ims);
	void initVAO(InitMapSetting& ims);
public:
	MapVAO(const std::string& map_id,TextureShaderProgram& s):
		mapId(map_id),
		texture(GL_TEXTURE0),
		vao(s,texture)
	{
	}
	void init(DBAccessor& dba);
	void display(void);
};





#endif