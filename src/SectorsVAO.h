/*
 * =====================================================================================
 *
 *       Filename:  SectorsVAO.h
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

#include "OpenGLHeaders.h"


#include "GCS.h"
#include "DBAccessor.h"
#include "Util.h"

#include "BasicShaderProgram.h"

class SectorsVAO
{
private:
	//クラス内クラス・構造体
	struct SubSector
	{
		int minimumAltitude;
		bool includesMinimumAltitude;
		int maximumAltitude;
		bool includesMaximumAltitude;
		std::vector<unsigned int> longLatIndex;
		//縁取り用の頂点
		std::vector<unsigned int> insideLongLatIndex;
	};
	struct Sector
	{
		std::string sectorName;
		std::string caption;
		std::string unitProvidingService;
		int subSectorCount;
		std::vector<SubSector> subSector;
	};
	/**
	 * @brief VAOに設定するための情報を格納する
	 */
	struct InitSectorsSettings
	{
		std::vector<Sector> sector;
		std::vector<LongLat> longLat;
		std::vector<glm::vec3> position;/**< 位置 */
		std::vector<glm::vec3> color;/**< 頂点色*/
		std::vector<unsigned int> element;/**< インデックス配列  */
	};
	//データメンバ
	bool displayAll;
	unsigned int displaySectorIndex;
	BasicShaderProgram::vaoType vao;
	//メンバ関数
	LongLat getInsideLongLat(LongLat& xi, LongLat& xj, LongLat& xk, double d);
	void getSectors(DBAccessor& dba, InitSectorsSettings& iss);
	void getSubSectors(DBAccessor& dba, Sector& s, InitSectorsSettings& iss);
	void getSubSectorVertex(DBAccessor& dba, Sector& s, int subsector_index, InitSectorsSettings& iss);
	void initSector(Sector& ss, InitSectorsSettings& iss);
	void initSubSector(const std::string& sector_name, SubSector& ss, const glm::vec3& color, InitSectorsSettings& iss);
	void addPositionColorIndex
		(
			const std::vector<unsigned int>& index,
			float altitude,
			const glm::vec3& color,
			InitSectorsSettings& iss,
			std::vector<unsigned int>& target_index
		);
	void setElement(const std::vector<unsigned int>& index, InitSectorsSettings& iss);
	void initVAO(InitSectorsSettings& iss);
public:
	SectorsVAO(BasicShaderProgram& b)
		:
			displayAll(true),
			displaySectorIndex(0),
			vao(b)
	{
	}
	void init(DBAccessor& dba)
	{
		InitSectorsSettings iss;
		
		getSectors(dba, iss);
		
		initVAO(iss);
		
		//デバッグ用の出力//////////////////////
		/*
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
		*/
		/////////////////////////////
	}
	/*
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
			if(displaySectorIndex >= sector.size())
			{
				displayAll = true;
				displaySectorIndex = 0;
			}
		}
	}*/
	void display()
	{
		vao.display();
	}
};





#endif
