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
	
	glBegin(GL_LINE_STRIP);
	glColor3d(1.0,0.0,0.0);
	for(int k = 0;k < kmax; k++)
	{
		LongLat& ll = longLat[ss.longLatIndex[k]];
		glVertex3d(ll.longitude,ll.latitude,((double)ss.maximumAltitude));
	}
	glVertex3d(ll0.longitude,ll0.latitude,((double)ss.maximumAltitude));
	glEnd();
}
