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

#include <algorithm>
#include <vector>
#include <string>
#include <cmath>

#include <GL/glut.h>

#include "GCS.h"
#include "DBAccessor.h"
#include "Util.h"


struct SubSector
{
	int minimumAltitude;
	bool includesMinimumAltitude;
	int maximumAltitude;
	bool includesMaximumAltitude;
	std::vector<int> longLatIndex;
	//縁取り用の頂点
	std::vector<int> insideLongLatIndex;
};

struct Sector
{
	std::string sectorName;
	std::string caption;
	std::string unitProvidingService;
	int subSectorCount;
	std::vector<SubSector> subSector;
};

class Sectors
{
private:
	std::vector<Sector> sector;
	std::vector<LongLat> longLat;
	LongLat getInsideLongLat(LongLat& xi, LongLat& xj, LongLat& xk, double d);
	void getSectors(DBAccessor& dba);
	void getSubSectors(DBAccessor& dba, Sector& s);
	void getSubSectorVertex(DBAccessor& dba, Sector& s, int subsector_index);
public:
	Sectors():sector(),longLat()
	{
	}
	void init(DBAccessor& dba)
	{
		getSectors(dba);
		
		/////////////////////////////
		return;
		int imax = sector.size();
		std::cout << "sector.size() = " << imax << std::endl;
		for(int i = 0; i < imax; i++)
		{
			std::cout << "sector[" << i << "]" << std::endl;
			std::cout << "\t" << "sectorName  = " << sector[i].sectorName << std::endl;
			std::cout << "\t" << "caption = " << sector[i].caption << std::endl;
			std::cout << "\t" << "unitProvidingService = " << sector[i].unitProvidingService << std::endl;
			std::cout << "\t" << "subSectorCount = " << sector[i].subSectorCount << std::endl;
			int jmax = sector[i].subSector.size();
			for(int j = 0; j < jmax; j++)
			{
				SubSector& ss = sector[i].subSector[j];
				std::cout << "\t" << "subSector[" << j << "]" << std::endl;
				std::cout << "\t\t" << "minimumAltitude = " << ss.minimumAltitude << std::endl;
				std::cout << "\t\t" << "includesMinimumAltitude = " << ss.includesMinimumAltitude << std::endl;
				std::cout << "\t\t" << "maximumAltitude = " << ss.maximumAltitude << std::endl;
				std::cout << "\t\t" << "includesMaximumAltitude = " << ss.includesMaximumAltitude << std::endl;
				int kmax = ss.longLatIndex.size();
				std::cout << "\t\t" << "longLatIndex.size() = " << kmax << std::endl;
				for(int k = 0; k < kmax; k++)
				{
					LongLat ll = longLat[ss.longLatIndex[k]];
					std::cout << "\t\t\t"<< ll.longitude << ", " << ll.latitude << std::endl;
				}
			}
		}
		/////////////////////////////
		/*
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
			ll = getInsideLongLat(llj,llk,lll,0.05);
			longLat.push_back(ll);
			ss.insideLongLatIndex.push_back(longLat.size()-1);
		}
		Sector s;
		s.sectorName=std::string("S01");
		s.subSector.push_back(ss);
		sector.push_back(s);
		*/
		/////////////////////////////
	}
	void display(void);
	void displaySubSector(SubSector& ss);
};





#endif
