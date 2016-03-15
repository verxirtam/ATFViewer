/*
 * =====================================================================================
 *
 *       Filename:  Sectors.cpp
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

#include "Sectors.h"

LongLat Sectors::getInsideLongLat(LongLat& xi, LongLat& xj, LongLat& xk, double distance)
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

void Sectors::getSectors(DBAccessor& dba)
{
	//クエリの作成
	std::stringstream sql("");
	sql << "select SectorName, Caption, UnitProvidingService, SubSectorCount from Sector;";

	dba.setQuery(sql.str());

	//格納前に初期化しておく
	sector.clear();

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
		sector.push_back(s);
	}
	//セクタ毎の情報の取得
	int imax = sector.size();
	for(int i = 0; i < imax; i++)
	{
		//サブセクタの取得
		getSubSectors(dba, sector[i]);
	}
}
void Sectors::getSubSectors(DBAccessor& dba, Sector& s)
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
			std::cout<<"error on Sectors::getSubSectors()"<<std::endl;
			return;
		}
		ssi++;
		SubSector ss;
		ss.minimumAltitude = Util::getFeetFromFL(dba.getColumnString(1));
		ss.includesMinimumAltitude = ( dba.getColumnInt(2) == 0 ) ? false : true;
		ss.maximumAltitude = Util::getFeetFromFL(dba.getColumnString(3));
		ss.includesMaximumAltitude = ( dba.getColumnInt(4) == 0 ) ? false : true;
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
		getSubSectorVertex(dba, s, j);
	}
}
void Sectors::getSubSectorVertex(DBAccessor& dba, Sector& s, int subsector_index)
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

	std::cout << sql.str() << std::endl;
	
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
			std::cout<<"error on Sectors::getSubSectorVertex()"<<std::endl;
			return;
		}
		vi++;
		LongLat ll;
		ll.longitude = Util::getLongitudeFromDMS(dba.getColumnString(1));
		ll.latitude  = Util::getLatitudeFromDMS( dba.getColumnString(2));
		//TODO IsCircle, Center, Radiusの取得処理を記述する
		//頂点配列に追加
		longLat.push_back(ll);
		//追加した頂点のインデックスを保存する
		ss.longLatIndex.push_back(longLat.size()-1);
	}
	
	int imax = ss.longLatIndex.size();
	std::cout << "imax = " << imax << std::endl;
	for(int i = 0; i < imax; i++)
	{
		int j = (i - 1 + imax) % imax;
		int k = (i    ) % imax;
		int l = (i + 1) % imax;
		std::cout << "(j,k,l)=(" << j << "," << k << "," << l << ")" << std::endl;
		LongLat llj = longLat[ss.longLatIndex[j]];
		LongLat llk = longLat[ss.longLatIndex[k]];
		LongLat lll = longLat[ss.longLatIndex[l]];
		LongLat ll = getInsideLongLat(llj,llk,lll,0.05);
		longLat.push_back(ll);
		ss.insideLongLatIndex.push_back(longLat.size()-1);
	}
}

void Sectors::display(void)
{
	int imax = sector.size();
	for(int i = 0; i < imax; i++)
	{
		int jmax = sector[i].subSector.size();
		for(int j = 0; j < jmax; j++)
		{
			Sectors::displaySubSector(sector[i].subSector[j]);
		}
	}
}

void Sectors::displaySubSector(SubSector& ss)
{
	int kmax = ss.longLatIndex.size();
	if(kmax == 0)
	{
		return;
	}
	
	//下底
	glBegin(GL_LINE_STRIP);
	glColor3d(1.0,0.0,0.0);
	for(int k = 0;k < kmax; k++)
	{
		LongLat& ll = longLat[ss.longLatIndex[k]];
		glVertex3d(ll.longitude,ll.latitude,((double)ss.minimumAltitude));
	}
	LongLat& ll0 = longLat[ss.longLatIndex[0]];
	glVertex3d(ll0.longitude,ll0.latitude,((double)ss.minimumAltitude));
	glEnd();
	//下底の内側
	glBegin(GL_LINE_STRIP);
	glColor3d(1.0,0.0,0.0);
	for(int k = 0;k < kmax; k++)
	{
		LongLat& ll = longLat[ss.insideLongLatIndex[k]];
		glVertex3d(ll.longitude,ll.latitude,((double)ss.minimumAltitude));
	}
	LongLat& lli0 = longLat[ss.insideLongLatIndex[0]];
	glVertex3d(lli0.longitude,lli0.latitude,((double)ss.minimumAltitude));
	glEnd();
	
	//上底
	glBegin(GL_LINE_STRIP);
	glColor3d(1.0,0.0,0.0);
	for(int k = 0;k < kmax; k++)
	{
		LongLat& ll = longLat[ss.longLatIndex[k]];
		glVertex3d(ll.longitude,ll.latitude,((double)ss.maximumAltitude));
	}
	glVertex3d(ll0.longitude,ll0.latitude,((double)ss.maximumAltitude));
	glEnd();
	
	//上底の内側
	glBegin(GL_LINE_STRIP);
	glColor3d(1.0,0.0,0.0);
	for(int k = 0;k < kmax; k++)
	{
		LongLat& ll = longLat[ss.insideLongLatIndex[k]];
		glVertex3d(ll.longitude,ll.latitude,((double)ss.maximumAltitude));
	}
	glVertex3d(lli0.longitude,lli0.latitude,((double)ss.maximumAltitude));
	glEnd();
	
	//側面
	glBegin(GL_LINES);
	for(int k = 0;k < kmax; k++)
	{
		LongLat& ll = longLat[ss.longLatIndex[k]];
		glVertex3d(ll.longitude,ll.latitude,((double)ss.minimumAltitude));
		glVertex3d(ll.longitude,ll.latitude,((double)ss.maximumAltitude));
	}
	glEnd();
	
}



