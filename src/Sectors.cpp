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



