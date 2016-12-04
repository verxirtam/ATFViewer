/*
 * =====================================================================================
 *
 *       Filename:  SectorsVAO.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年03月11日 01時52分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "SectorsVAO.h"

LongLat SectorsVAO::getInsideLongLat(LongLat& xi, LongLat& xj, LongLat& xk, double distance)
{
	double length_i;
	double length_j;
	LongLat normal_i;
	LongLat normal_j;
	//xj-xiを左に90度回転したベクトル
	normal_i.longitude = - (xj.latitude  - xi.latitude );
	normal_i.latitude  =    xj.longitude - xi.longitude;
	//長さを求める
	length_i = std::sqrt( normal_i.longitude * normal_i.longitude + normal_i.latitude * normal_i.latitude );
	//長さで割って正規化する
	normal_i.longitude /= length_i;
	normal_i.latitude  /= length_i;
	
	//xk-xjを左に90度回転したベクトル
	normal_j.longitude = - (xk.latitude  - xj.latitude );
	normal_j.latitude  =    xk.longitude - xj.longitude;
	//長さを求める
	length_j = std::sqrt( normal_j.longitude * normal_j.longitude + normal_j.latitude * normal_j.latitude );
	//長さで割って正規化する
	normal_j.longitude /= length_j;
	normal_j.latitude  /= length_j;
	
	//以下の一次方程式を解く
	//(a b)(x)=(e)
	//(c d)(y) (f)
	double a = normal_i.longitude;
	double b = normal_i.latitude;
	double c = normal_j.longitude;
	double d = normal_j.latitude;
	double e = distance + xi.longitude * normal_i.longitude + xi.latitude * normal_i.latitude;
	double f = distance + xj.longitude * normal_j.longitude + xj.latitude * normal_j.latitude;
	
	double det = a * d - b * c;
	
	if(det == 0.0)
	{
		LongLat ret;
		ret.longitude = xj.longitude + normal_i.longitude * distance;
		ret.latitude  = xj.latitude  + normal_i.latitude  * distance;
		return ret;
	}
	LongLat ret;
	double ia =   d;
	double ib = - b;
	double ic = - c;
	double id =   a;
	ret.longitude = ( ia * e + ib * f ) / det;
	ret.latitude  = ( ic * e + id * f ) / det;
	return ret;
}

void SectorsVAO::getSectors(DBAccessor& dba, InitSectorsSettings& iss)
{
	//クエリの作成
	std::stringstream sql("");
	sql << "select SectorName, Caption, UnitProvidingService, SubSectorCount from Sector;";

	dba.setQuery(sql.str());

	//格納前に初期化しておく
	iss.sector.clear();

	//クエリの実行
	while(SQLITE_ROW == dba.step_select())
	{
		//結果の取得
		Sector s;
		s.sectorName = dba.getColumnString(0);
		s.caption = dba.getColumnString(1);
		s.unitProvidingService = dba.getColumnString(2);
		s.subSectorCount = dba.getColumnInt(3);
		//sectorに格納
		iss.sector.push_back(s);
	}
	//セクタ毎の情報の取得
	int imax = iss.sector.size();
	for(int i = 0; i < imax; i++)
	{
		//サブセクタの取得
		getSubSectors(dba, iss.sector[i], iss);
	}
}
void SectorsVAO::getSubSectors(DBAccessor& dba, Sector& s, InitSectorsSettings& iss)
{
	//クエリの作成
	std::stringstream sql("");
	sql << "select ";
	sql << "SubSectorIndex, MinimumAltitude, IncludesMinimumAltitude, MaximumAltitude, IncludesMaximumAltitude ";
	sql << "from SubSector ";
	sql << "where SectorName = '" << s.sectorName << "' ";
	sql << "order by SubSectorIndex";
	sql << ";";

	dba.setQuery(sql.str());

	s.subSector.clear();

	int ssi = 0;
	
	while(SQLITE_ROW == dba.step_select())
	{
		//SubSectorIndexのチェック
		int subsector_index =  dba.getColumnInt(0);
		if(ssi != subsector_index)
		{
			std::cout<<"error on SectorsVAO::getSubSectors()"<<std::endl;
			return;
		}
		ssi++;
		SubSector ss;
		ss.minimumAltitude = Util::getFeetFromFL(dba.getColumnString(1));
		ss.includesMinimumAltitude = ( dba.getColumnInt(2) == 0 ) ? false : true;
		ss.maximumAltitude = Util::getFeetFromFL(dba.getColumnString(3));
		ss.includesMaximumAltitude = ( dba.getColumnInt(4) == 0 ) ? false : true;
		if(ss.minimumAltitude < 1000)
		{
			ss.minimumAltitude = 1000;
		}
		if(ss.maximumAltitude >= 50000)
		{
			ss.maximumAltitude = 50000;
		}
		//サブセクタの格納
		s.subSector.push_back(ss);
	}
	int jmax = s.subSector.size();
	for(int j = 0; j < jmax; j++)
	{
		//サブセクタの頂点配列の取得
		getSubSectorVertex(dba, s, j, iss);
	}
}
void SectorsVAO::getSubSectorVertex(DBAccessor& dba, Sector& s, int subsector_index, InitSectorsSettings& iss)
{
	//クエリの作成
	std::stringstream sql("");
	sql << "select ";
	sql << "VertexIndex, LongitudeDMS, LatitudeDMS, IsCircle, Center, Radius ";
	sql << "from SubSectorVertex ";
	sql << "where SectorName = '" << s.sectorName << "' ";
	sql << "and SubSectorIndex = " << subsector_index << " ";
	sql << "order by VertexIndex";
	sql << ";";

	//std::cout << sql.str() << std::endl;
	
	dba.setQuery(sql.str());
	
	SubSector& ss = s.subSector[subsector_index];
	
	ss.longLatIndex.clear();
	
	int vi = 0;
	
	while(SQLITE_ROW == dba.step_select())
	{
		//VertexIndexのチェック
		int vertex_index = dba.getColumnInt(0);
		if(vi != vertex_index)
		{
			std::cout<<"error on SectorsVAO::getSubSectorVertex()"<<std::endl;
			return;
		}
		vi++;
		LongLat ll;
		ll.longitude = Util::getLongitudeFromDMS(dba.getColumnString(1));
		ll.latitude  = Util::getLatitudeFromDMS( dba.getColumnString(2));
		//TODO IsCircle, Center, Radiusの取得処理を記述する
		//頂点配列に追加
		iss.longLat.push_back(ll);
		//追加した頂点のインデックスを保存する
		ss.longLatIndex.push_back(iss.longLat.size()-1);
	}
	
	//頂点配列が左回りかどうかを判定する
	if(isClockwise(ss.longLatIndex, iss.longLat))
	{
		//右回りの場合はインデックス配列を逆順に変更する
		std::reverse(ss.longLatIndex.begin(),ss.longLatIndex.end());
	}
	
	
	int imax = ss.longLatIndex.size();
	for(int i = 0; i < imax; i++)
	{
		int j = (i - 1 + imax) % imax;
		int k = (i    ) % imax;
		int l = (i + 1) % imax;
		LongLat llj = iss.longLat[ss.longLatIndex[j]];
		LongLat llk = iss.longLat[ss.longLatIndex[k]];
		LongLat lll = iss.longLat[ss.longLatIndex[l]];
		LongLat ll = getInsideLongLat(llj,llk,lll,0.025);
		iss.longLat.push_back(ll);
		ss.insideLongLatIndex.push_back(iss.longLat.size()-1);
	}
}

void SectorsVAO::initVAO(InitSectorsSettings& iss)
{
	
	if(displayAll)
	{
		int imax = iss.sector.size();
		for(int i = 0; i < imax; i++)
		{
			initSector(iss.sector[i], iss);
		}
	}
	else
	{
		initSector(iss.sector[displaySectorIndex], iss);
	}
	
	//vao.init()向けにstd::vector<float>に詰め替える
	vaoType::inputType v;
	std::vector<float>& position_align = v.position;
	std::vector<float>&    color_align = v.color;
	
	int kmax = iss.position.size();
	for(int k = 0; k < kmax; k++)
	{
		position_align.push_back(iss.position[k].x);
		position_align.push_back(iss.position[k].y);
		position_align.push_back(iss.position[k].z);
	}
	
	kmax = iss.color.size();
	for(int k = 0; k < kmax; k++)
	{
		color_align.push_back(iss.color[k].x);
		color_align.push_back(iss.color[k].y);
		color_align.push_back(iss.color[k].z);
	}
	//vaoとしての初期化
	vao.init(v, iss.element, GL_LINES);
	
	std::cout << "iss.position.size() = " << iss.position.size() << std::endl;
	std::cout << "position_align.size() = " << position_align.size() << std::endl;
	std::cout << "iss.color.size() = " << iss.color.size() << std::endl;
	std::cout << "color_align.size() = " << color_align.size() << std::endl;
	std::cout << "iss.element.size() = " << iss.element.size() << std::endl;
}

void SectorsVAO::initSector(Sector& sector, InitSectorsSettings& iss)
{
	glm::vec3 color;
	
	//色の設定
	switch (sector.sectorName[0])
	{
	case 'S':
		color = glm::vec3(1.0f , 0.0f , 0.0f);
		break;
	case 'T':
		color = glm::vec3(0.75f, 0.75f, 0.0f);
		break;
	case 'F':
		color = glm::vec3(0.0f , 1.0f , 0.0f);
		break;
	case 'N':
		color = glm::vec3(0.0f , 1.0f , 1.0f);
		break;
	case 'A':
		color = glm::vec3(0.0f , 0.0f , 1.0f);
		break;
	}
	int jmax = sector.subSector.size();
	for(int j = 0; j < jmax; j++)
	{
		initSubSector(sector.sectorName + std::string(" ") + sector.caption , sector.subSector[j], color, iss);
	}
}

void SectorsVAO::addPositionColorIndex
	(
		const std::vector<unsigned int>& index,
		float altitude,
		const glm::vec3& color,
		InitSectorsSettings& iss,
		std::vector<unsigned int>& target_index
	)
{
	int kmax = index.size();
	for(int k = 0;k < kmax; k++)
	{
		//経緯度を取得
		LongLat& ll = iss.longLat[index[k]];
		//positionに追加
		iss.position.push_back(glm::vec3(ll.longitude, ll.latitude, altitude));
		//colorに追加
		iss.color.push_back(glm::vec3(color));
		//追加したpositionのインデックスを保存
		target_index.push_back(iss.position.size()-1);
	}
}

void SectorsVAO::setElement(const std::vector<unsigned int>& index, InitSectorsSettings& iss)
{
	int kmax = index.size();
	for(int k = 0;k < kmax - 1; k++)
	{
		iss.element.push_back(index[k    ]);//始点
		iss.element.push_back(index[k + 1]);//終点
	}
	iss.element.push_back(index[kmax - 1]);//始点
	iss.element.push_back(index[0       ]);//終点
	
}

void SectorsVAO::initSubSector(const std::string& sector_name, SubSector& ss, const glm::vec3& color, InitSectorsSettings& iss)
{
	int kmax = ss.longLatIndex.size();
	if(kmax < 3)
	{
		return;
	}
	
	//下底のインデックス
	std::vector<unsigned int> index_low;
	//下底の内側のインデックス
	std::vector<unsigned int> index_low_inside;
	//上底のインデックス
	std::vector<unsigned int> index_high;
	//上底の内側のインデックス
	std::vector<unsigned int> index_high_inside;
	
	//頂点を登録する
	//登録した頂点のインデックスを保持しておく
	//下底
	addPositionColorIndex(ss.longLatIndex,       ss.minimumAltitude, color, iss, index_low        );
	//下底の内側
	addPositionColorIndex(ss.insideLongLatIndex, ss.minimumAltitude, color, iss, index_low_inside );
	//上底
	addPositionColorIndex(ss.longLatIndex,       ss.maximumAltitude, color, iss, index_high       );
	//上底の内側
	addPositionColorIndex(ss.insideLongLatIndex, ss.maximumAltitude, color, iss, index_high_inside);
	
	
	//インデックス配列の設定
	//下底
	setElement(index_low        , iss);
	//下底の内側
	setElement(index_low_inside , iss);
	//上底
	setElement(index_high       , iss);
	//上底の内側
	setElement(index_high_inside, iss);
	
	//側面
	for(int k = 0;k < kmax - 1; k++)
	{
		iss.element.push_back(index_low [k]);
		iss.element.push_back(index_high[k]);
	}
	
	
	//セクタ名の表示
	StringVAO::SingleString single_string;
	LongLat ll = iss.longLat[ss.longLatIndex[0]];
	float altitude = ((float)ss.maximumAltitude)+1000.0f;
	single_string.position = glm::vec3(ll.longitude, ll.latitude, altitude);
	single_string.horisontal = glm::vec3(0.125f, 0.0f, 0.0f);
	single_string.vertical = glm::vec3(0.0f, 0.125f, 5000.0f);
	single_string.dispString = sector_name;
	iss.sectorName.push_back(single_string);
	//BitmapString::drawString(lli0.longitude, lli0.latitude, ((float)ss.maximumAltitude)+1000.0,sector_name.c_str());
}



