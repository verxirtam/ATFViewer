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
	std::string map_id("ENRC1_20160204");//"bm_200406");
	std::string texture_file_name;

	getSettings(dba,map_id,texture_file_name);
	initTexture(texture_file_name);
	getVertex(dba,map_id);
	getVertexIndex(dba,map_id);
	
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
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//テクスチャの割り当て
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, textureWidth, textureHeight, GL_RGB , GL_UNSIGNED_BYTE, texture);
	//テクスチャを拡大・縮小するときの保管方法を設定 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	
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
	//モデリング変換行列の設定
	//モデル座標は経緯度-feetのサイズになっているので
	//地図っぽくスケーリングする
	//経緯度はそのままで行けそうなので、feetのみ変更
	//40000feetが4cmになるように1/10000に縮小する
	glScaled(scale,scale,0.0001);
	//羽田+offsetがワールド座標系の原点に来るように平行移動する
	double haneda_x = 139.0 + ( 46.0/60.0 + 87.0/3600.0 );
	double haneda_y =  35.0 + ( 33.0/60.0 + 20.0/3600.0 );
	glTranslated( - haneda_x - centerOffsetLong, - haneda_y - centerOffsetLat, 0.0);
	
	//世界地図を描く
	glColor3d(1.0,1.0,1.0);
	glEnable(GL_TEXTURE_2D);
	/*
	glBegin(GL_TRIANGLES);
	GLdouble tex_coord[][2]={
								{ 72.2/1024.0,  71.2/1024.0},
								{991.4/1024.0,  22.0/1024.0},
								{198.2/1024.0, 893.5/1024.0},
								{981.3/1024.0, 893.1/1024.0}
							};
	GLdouble ver_coord[][3]={
								{138.0, 34.0 + 58.0/60.0, 0.0},
								{140.0, 38.0 + 30.0/60.0, 0.0},
								{142.0, 33.0 + 48.0/60.0, 0.0},
								{144.0, 36.0 + 40.0/60.0, 0.0}
							};
	//0,2,1   1,2,3
	glTexCoord2d(tex_coord[0][0],tex_coord[0][1]);glVertex3d(ver_coord[0][0],ver_coord[0][1],ver_coord[0][2]);
	glTexCoord2d(tex_coord[1][0],tex_coord[1][1]);glVertex3d(ver_coord[1][0],ver_coord[1][1],ver_coord[1][2]);
	glTexCoord2d(tex_coord[2][0],tex_coord[2][1]);glVertex3d(ver_coord[2][0],ver_coord[2][1],ver_coord[2][2]);
	
	glTexCoord2d(tex_coord[1][0],tex_coord[1][1]);glVertex3d(ver_coord[1][0],ver_coord[1][1],ver_coord[1][2]);
	glTexCoord2d(tex_coord[2][0],tex_coord[2][1]);glVertex3d(ver_coord[2][0],ver_coord[2][1],ver_coord[2][2]);
	glTexCoord2d(tex_coord[3][0],tex_coord[3][1]);glVertex3d(ver_coord[3][0],ver_coord[3][1],ver_coord[3][2]);
	glEnd();
	*/
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
	
	
	glDisable(GL_TEXTURE_2D);

	//罫線を描画する
	glBegin(GL_LINES);
	glColor3d(1.0,0.0,0.0);
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

