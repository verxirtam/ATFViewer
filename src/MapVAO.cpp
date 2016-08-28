/*
 * =====================================================================================
 *
 *       Filename:  MapVAO.cpp
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
	//std::string textureFileName;
	InitMapSetting ims;

	getSettings(dba, mapId, ims);
	texture.init(ims.textureFileName, ims.textureWidth, ims.textureHeight);
	getVertex(dba, mapId, ims);
	getVertexIndex(dba, mapId, ims);
	initVAO(ims);
}

void MapVAO::getSettings(DBAccessor& dba,std::string& map_id, InitMapSetting& ims)
{
	std::stringstream sql("");
	sql << "select textureFileName, width, height from Map where mapId='";
	sql << map_id;
	sql << "';";

	dba.setQuery(sql.str());

	if(SQLITE_ROW == dba.step_select())
	{
		ims.textureFileName = dba.getColumnString(0);
		ims.textureWidth = dba.getColumnInt(1);
		ims.textureHeight = dba.getColumnInt(2);
	}

}

void MapVAO::getVertex(DBAccessor& dba,std::string& map_id, InitMapSetting& ims)
{
	//クエリの作成
	std::stringstream sql("");
	sql << "select vertexIndex, longitudeDMS, latitudeDMS, u, v from MapVertex where mapId='";
	sql << map_id;
	sql << "' order by vertexIndex;";

	dba.setQuery(sql.str());
	
	//mapVertexの内容を全て削除する
	ims.mapVertex.clear();

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
		mv.u =((float)(dba.getColumnInt(3)))/((float)(ims.textureWidth));
		mv.v =((float)(dba.getColumnInt(4)))/((float)(ims.textureHeight));
		ims.mapVertex.push_back(mv);
	}
}
void MapVAO::getVertexIndex(DBAccessor& dba,std::string& map_id, InitMapSetting& ims)
{
	
	//クエリの作成
	std::stringstream sql("");
	sql << "select triangleStripIndex, vertexIndexIndex, vertexIndex from MapVertexIndex where mapId='";
	sql << map_id;
	sql << "' order by triangleStripIndex, vertexIndexIndex;";
	
	dba.setQuery(sql.str());
	
	//mapVertexIndexの内容を全て削除する
	ims.mapVertexIndex.clear();
	
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
			ims.mapVertexIndex.push_back(std::vector<MapVertexIndex>());
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
		ims.mapVertexIndex[tsindex].push_back(mvi);
	}
}

void MapVAO::initVAO(InitMapSetting& ims)
{
	vaoType::inputType input;
	
	std::vector<float>& p = input.position;
	std::vector<float>& t = input.texture;
	
	std::vector<unsigned int> e;
	
	int imax = ims.mapVertex.size();
	for(int i = 0; i < imax; i++)
	{
		p.push_back((float)ims.mapVertex[i].longitude);
		p.push_back((float)ims.mapVertex[i].latitude);
		p.push_back(0.0f);
		
		t.push_back((float)ims.mapVertex[i].u);
		t.push_back((float)ims.mapVertex[i].v);
	}
	
	imax = ims.mapVertexIndex.size();
	for(int i = 0; i < imax; i++)
	{
		int jmax = ims.mapVertexIndex[i].size();
		if(( 0 < i ) && ( 0 < jmax ))
		{
			e.push_back(ims.mapVertexIndex[i][0].vertexIndex);
		}
		for (int j = 0; j < jmax; j++)
		{
			e.push_back(ims.mapVertexIndex[i][j].vertexIndex);
		}
		if((i < imax - 1) && ( 0 < jmax ))
		{
			e.push_back(ims.mapVertexIndex[i][jmax -1].vertexIndex);
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
	
	this->vao.init(input,e,GL_TRIANGLE_STRIP);
}
void MapVAO::display(void)
{
	this->vao.display();
}

