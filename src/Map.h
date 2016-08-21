/*
 * =====================================================================================
 *
 *       Filename:  Map.h
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




class Map
{
private:
	struct MapVertex
	{
		double longitude;
		double latitude;
		double u;
		double v;
	};
	
	struct MapVertexIndex
	{
		int vertexIndex;
	};
	std::string mapId;
	GLuint textureName[1];
	int textureWidth;
	int textureHeight;
	std::vector<MapVertex> mapVertex;
	std::vector<std::vector<MapVertexIndex> > mapVertexIndex;
public:
	Map(const std::string& map_id)
		:
			mapId(map_id),
			textureName{0},
			textureWidth(0),
			textureHeight(0),
			mapVertex(),
			mapVertexIndex()
	{
	}
	void init(DBAccessor& dba);
	void getSettings(DBAccessor& dba, std::string& map_id, std::string& texture_file_name);
	void initTexture(std::string& texture_file_name);
	void getVertex(DBAccessor& dba,std::string& map_id);
	void getVertexIndex(DBAccessor& dba,std::string& map_id);
	void display(void);
};





#endif
