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

#include "BitmapString.h"
#include "GCS.h"
#include "DBAccessor.h"
#include "Util.h"


class Sectors
{
private:
	struct SubSector
	{
		int minimumAltitude;
		bool includesMinimumAltitude;
		int maximumAltitude;
		bool includesMaximumAltitude;
		std::vector<unsigned int> longLatIndex;
		//縁取り用の頂点
		std::vector<unsigned int> insideLongLatIndex;
		SubSector()
			:
				minimumAltitude(0),
				includesMinimumAltitude(true),
				maximumAltitude(50000),
				includesMaximumAltitude(true),
				longLatIndex(),
				insideLongLatIndex()
		{
		}
	};
	struct Sector
	{
		std::string sectorName;
		std::string caption;
		std::string unitProvidingService;
		int subSectorCount;
		std::vector<SubSector> subSector;
		Sector()
			:
				sectorName(),
				caption(),
				unitProvidingService(),
				subSectorCount(0),
				subSector()
		{
		}
	};
	std::vector<Sector> sector;
	std::vector<LongLat> longLat;
	bool displayAll;
	unsigned int displaySectorIndex;
	LongLat getInsideLongLat(LongLat& xi, LongLat& xj, LongLat& xk, double d);
	void getSectors(DBAccessor& dba);
	void getSubSectors(DBAccessor& dba, Sector& s);
	void getSubSectorVertex(DBAccessor& dba, Sector& s, int subsector_index);
	void displaySector(Sector& ss);
	void displaySubSector(const std::string& sector_name, SubSector& ss);
public:
	Sectors():sector(),longLat(),displayAll(true),displaySectorIndex(0)
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
	}
	void display(void);

	void switchDisplaySector()
	{
		if(displayAll)
		{
			displayAll = false;
			displaySectorIndex = 0;
		}
		else
		{
			displaySectorIndex++;
			if(displaySectorIndex>=sector.size())
			{
				displayAll = true;
				displaySectorIndex = 0;
			}
		}

	}
};





#endif
