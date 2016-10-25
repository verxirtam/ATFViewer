/*
 * =====================================================================================
 *
 *       Filename:  Map.cpp
 *
 *    Description:  マップを描画する
 *
 *        Version:  1.0
 *        Created:  2016年02月07日 17時27分01秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "Map.h"

void Map::init(DBAccessor& dba)
{
	std::string texture_file_name;

	getSettings(dba,mapId,texture_file_name);
	initTexture(texture_file_name);
	getVertex(dba,mapId);
	getVertexIndex(dba,mapId);
	
	
	
	
	/*
	//Vertexの内容を標準出力に表示する
	std::cout<<"mapVertex"<<std::endl;
	int imax = mapVertex.size();
	
	for(int i = 0; i < imax; i++)
	{
		std::cout << i << ", ";
		std::cout << mapVertex[i].longitude << ", ";
		std::cout << mapVertex[i].latitude << ", ";
		std::cout << mapVertex[i].u << ", ";
		std::cout << mapVertex[i].v;
		std::cout<<std::endl;
	}
	
	std::cout<<std::endl;
	std::cout<<"mapVertexIndex"<<std::endl;
	imax = mapVertexIndex.size();
	for(int i = 0; i < imax; i++)
	{
		std::cout << i << ": ";
		int jmax = mapVertexIndex[i].size();
		for (int j = 0; j < jmax; j++)
		{
			std::cout << mapVertexIndex[i][j].vertexIndex << ", ";
		}
		std::cout << std::endl;
	}
	*/
}

void Map::getSettings(DBAccessor& dba,std::string& map_id, std::string& texture_file_name)
{
	std::stringstream sql("");
	sql << "select textureFileName, width, height from Map where mapId='";
	sql << map_id;
	sql << "';";

	dba.setQuery(sql.str());

	if(SQLITE_ROW == dba.step_select())
	{
		texture_file_name = dba.getColumnString(0);
		textureWidth = dba.getColumnInt(1);
		textureHeight = dba.getColumnInt(2);
	}

}
void Map::initTexture(std::string& texture_file_name)
{
	
	GLubyte* texture = new GLubyte[textureWidth * textureHeight * 3];
	
	//テクスチャファイルのファイルポインタ
	std::FILE* fp;
	//テクスチャファイルの読み込み
	if ((fp = std::fopen(texture_file_name.c_str(), "rb")) != NULL)
	{
		//std::fread(texture, sizeof(texture),1,fp);
		std::fread(texture, sizeof(GLubyte) * textureWidth * textureHeight * 3,1,fp);
		std::fclose(fp);
	}
	else
	{
		printf("error.\n");
		std::perror(texture_file_name.c_str());
	}
	//テクスチャ名の取得
	glGenTextures(1, textureName);
	
	//画像データの格納方式を指定する(1画素が何バイト単位かを指定)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//設定するテクスチャの指定
	glBindTexture(GL_TEXTURE_2D, textureName[0]);
	
	//テクスチャの割り当て
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, textureWidth, textureHeight, GL_RGB , GL_UNSIGNED_BYTE, texture);
	//テクスチャを拡大・縮小するときの保管方法を設定 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	//無名テクスチャに指定し直す
	glBindTexture(GL_TEXTURE_2D, 0);
	
	delete[] texture;
}

void Map::getVertex(DBAccessor& dba,std::string& map_id)
{
	//クエリの作成
	std::stringstream sql("");
	sql << "select vertexIndex, longitudeDMS, latitudeDMS, u, v from MapVertex where mapId='";
	sql << map_id;
	sql << "' order by vertexIndex;";

	dba.setQuery(sql.str());
	
	//mapVertexの内容を全て削除する
	mapVertex.clear();

	//インデックスのチェック用
	int vindex = 0;
	
	while(SQLITE_ROW == dba.step_select())
	{
		//vertexIndexのチェック
		int vertex_index = dba.getColumnInt(0);
		if(vindex != vertex_index)
		{
			std::cout<<"error on Map::getVertex()"<<std::endl;
			return;
		}
		vindex++;
		//MapVertexの格納
		MapVertex mv;
		mv.longitude = Util::getLongitudeFromDMS(dba.getColumnString(1));
		mv.latitude = Util::getLatitudeFromDMS(dba.getColumnString(2));
		mv.u = ((double)(dba.getColumnInt(3)))/((double)(textureWidth));
		mv.v =((double)(dba.getColumnInt(4)))/((double)(textureHeight));
		mapVertex.push_back(mv);
	}
}
void Map::getVertexIndex(DBAccessor& dba,std::string& map_id)
{
	
	//クエリの作成
	std::stringstream sql("");
	sql << "select triangleStripIndex, vertexIndexIndex, vertexIndex from MapVertexIndex where mapId='";
	sql << map_id;
	sql << "' order by triangleStripIndex, vertexIndexIndex;";
	
	dba.setQuery(sql.str());
	
	//mapVertexIndexの内容を全て削除する
	mapVertexIndex.clear();
	
	//インデックスのチェック用
	int tsindex = -1;
	int viindex = 0;
	while(SQLITE_ROW == dba.step_select())
	{
		//triangleStripIndexのチェック
		int triangle_strip_index = dba.getColumnInt(0);
		if(triangle_strip_index == tsindex + 1)
		{
			tsindex++;
			mapVertexIndex.push_back(std::vector<MapVertexIndex>());
			viindex = 0;
		}
		//vertexIndexIndexのチェック
		int vertex_index_index = dba.getColumnInt(1);
		if(viindex != vertex_index_index)
		{
			std::cout<<"error on Map::getVertexIndex()"<<std::endl;
			return;
		}
		viindex++;
		//MapVertexIndexの格納
		MapVertexIndex mvi;
		mvi.vertexIndex = dba.getColumnInt(2);
		mapVertexIndex[tsindex].push_back(mvi);
	}
}

void Map::display(void)
{
	//緯度経度座標系での描画
	
	//世界地図を描く
	glColor3d(1.0,1.0,1.0);
	//設定するテクスチャの指定
	glBindTexture(GL_TEXTURE_2D, textureName[0]);
	//テクスチャマップを有効にする
	glEnable(GL_TEXTURE_2D);
	
	int imax = mapVertexIndex.size();
	for(int i = 0; i < imax; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		int jmax = mapVertexIndex[i].size();
		for (int j = 0; j < jmax; j++)
		{
			int vi = mapVertexIndex[i][j].vertexIndex;
			glTexCoord2d(mapVertex[vi].u, mapVertex[vi].v);
			glVertex3d(mapVertex[vi].longitude, mapVertex[vi].latitude, 0.0);
		}
		glEnd();
	}
	
	//テクスチャマップを無効にする
	glDisable(GL_TEXTURE_2D);
	//無名テクスチャに指定し直す
	glBindTexture(GL_TEXTURE_2D, 0);

	//罫線を描画する
	glBegin(GL_LINES);
	glColor3d(0.0,1.0,0.0);
	glVertex3d(137.0,34.0,10.0);glVertex3d(145.0,34.0,10.0);
	glVertex3d(137.0,35.0,10.0);glVertex3d(145.0,35.0,10.0);
	glVertex3d(137.0,36.0,10.0);glVertex3d(145.0,36.0,10.0);
	glVertex3d(137.0,37.0,10.0);glVertex3d(145.0,37.0,10.0);
	glVertex3d(137.0,38.0,10.0);glVertex3d(145.0,38.0,10.0);
	glVertex3d(137.0,39.0,10.0);glVertex3d(145.0,39.0,10.0);
	
	glVertex3d(138.0,33.0,10.0);glVertex3d(138.0,39.0,10.0);
	glVertex3d(139.0,33.0,10.0);glVertex3d(139.0,39.0,10.0);
	glVertex3d(140.0,33.0,10.0);glVertex3d(140.0,39.0,10.0);
	glVertex3d(141.0,33.0,10.0);glVertex3d(141.0,39.0,10.0);
	glVertex3d(142.0,33.0,10.0);glVertex3d(142.0,39.0,10.0);
	glVertex3d(143.0,33.0,10.0);glVertex3d(143.0,39.0,10.0);
	glVertex3d(144.0,33.0,10.0);glVertex3d(144.0,39.0,10.0);
	glEnd();
}

