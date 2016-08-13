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
#include "MapVAO.h"

void MapVAO::init(DBAccessor& dba)
{
	std::string texture_file_name;

	getSettings(dba,mapId,texture_file_name);
	texture.init(texture_file_name, textureWidth, textureHeight);
	getVertex(dba,mapId);
	getVertexIndex(dba,mapId);
	initVAO();
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

void MapVAO::getSettings(DBAccessor& dba,std::string& map_id, std::string& texture_file_name)
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

void MapVAO::getVertex(DBAccessor& dba,std::string& map_id)
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
			std::cout<<"error on MapVAO::getVertex()"<<std::endl;
			return;
		}
		vindex++;
		//MapVertexの格納
		MapVertex mv;
		mv.longitude = Util::getLongitudeFromDMS(dba.getColumnString(1));
		mv.latitude = Util::getLatitudeFromDMS(dba.getColumnString(2));
		mv.u =((float)(dba.getColumnInt(3)))/((float)(textureWidth));
		mv.v =((float)(dba.getColumnInt(4)))/((float)(textureHeight));
		mapVertex.push_back(mv);
	}
}
void MapVAO::getVertexIndex(DBAccessor& dba,std::string& map_id)
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
			std::cout<<"error on MapVAO::getVertexIndex()"<<std::endl;
			return;
		}
		viindex++;
		//MapVertexIndexの格納
		MapVertexIndex mvi;
		mvi.vertexIndex = dba.getColumnInt(2);
		mapVertexIndex[tsindex].push_back(mvi);
	}
}

void MapVAO::initVAO(void)
{
	
	std::vector<float> p;
	std::vector<float> t;
	
	std::vector<unsigned int> e;
	
	int imax = mapVertex.size();
	for(int i = 0; i < imax; i++)
	{
		p.push_back((float)mapVertex[i].longitude);
		p.push_back((float)mapVertex[i].latitude);
		p.push_back(0.0f);
		
		t.push_back((float)mapVertex[i].u);
		t.push_back((float)mapVertex[i].v);
	}
	
	imax = mapVertexIndex.size();
	for(int i = 0; i < imax; i++)
	{
		int jmax = mapVertexIndex[i].size();
		if(( 0 < i ) && ( 0 < jmax ))
		{
			e.push_back(mapVertexIndex[i][0].vertexIndex);
		}
		for (int j = 0; j < jmax; j++)
		{
			e.push_back(mapVertexIndex[i][j].vertexIndex);
		}
		if((i < imax - 1) && ( 0 < jmax ))
		{
			e.push_back(mapVertexIndex[i][jmax -1].vertexIndex);
		}
	}
	
	std::cout << "p.size() = " << p.size() << std::endl;
	std::cout << "t.size() = " << t.size() << std::endl;
	std::cout << "e.size() = " << e.size() << std::endl;
	
	//imax = p.size();
	//for(int i = 0; i < imax; i++)
	//{
	//	std::cout << p[i] << "\t";
	//}
	
	this->vao.init(p,t,e,GL_TRIANGLE_STRIP);
}
void MapVAO::display(void)
{
	this->vao.display();
}

