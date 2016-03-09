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
	string sectorName;
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
		ss.minimumAltitude=240;
		ss.includesMinimumAltitude=false;
		ss.maximumAltitude=999;
		ss.includesMaximumAltitude=false;
		
		LongLat ll;
		
		ll.longitude = Util::getLongitudeFromDMS("1403200.00E");ll.latitude = Util::getLatitudeFromDMS("390010.00N");
		longLat.push_back(ll);ss.longLatIndex.push_back(longLat.size()-1);

		/////////////////////////////
	}
};





#endif
