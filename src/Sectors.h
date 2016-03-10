/*
 * =====================================================================================
 *
 *       Filename:  Sectors.h
 *
 *    Description:  セクタを描画する
 *
 *        Version:  1.0
 *        Created:  2016年03月09日 21時23分58秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef Sectors_H
#define Sectors_H

#include <vector>
#include <string>

#include <GL/glut.h>

#include "Util.h"

struct LongLat
{
	double longitude;
	double latitude;
};

struct SubSector
{
	int minimumAltitude;
	bool includesMinimumAltitude;
	int maximumAltitude;
	bool includesMaximumAltitude;
	std::vector<int> longLatIndex;
};

struct Sector
{
	std::string sectorName;
	std::vector<SubSector> subSector;
};

class Sectors
{
private:
	std::vector<Sector> sector;
	std::vector<LongLat> longLat;

public:
	Sectors():sector(),longLat()
	{
		/////////////////////////////
		SubSector ss;
		ss.minimumAltitude=24000;
		ss.includesMinimumAltitude=false;
		ss.maximumAltitude=50000;
		ss.includesMaximumAltitude=false;
		
		LongLat ll;
		
		char long_dms[11][12]=
		{
			"1403200.00E",
			"1403313.00E",
			"1404114.00E",
			"1405037.00E",
			"1404344.00E",
			"1404307.00E",
			"1404417.00E",
			"1383131.00E",
			"1382442.00E",
			"1384548.00E",
			"1395948.00E"
		};
		char lat_dms[11][11]=
		{
			"390010.00N",
			"390817.00N",
			"393839.00N",
			"395302.00N",
			"395602.00N",
			"401410.00N",
			"402212.00N",
			"402212.00N",
			"393010.00N",
			"393010.00N",
			"390010.00N"
		};
		for(int i = 0; i<11; i++)
		{
			ll.longitude = Util::getLongitudeFromDMS(long_dms[i]);
			ll.latitude = Util::getLatitudeFromDMS(lat_dms[i]);
			longLat.push_back(ll);
			ss.longLatIndex.push_back(longLat.size()-1);
		}
		Sector s;
		s.sectorName=std::string("S01");
		s.subSector.push_back(ss);
		sector.push_back(s);
		/////////////////////////////
	}
	void display(void);
	void displaySubSector(SubSector& ss);
};





#endif
