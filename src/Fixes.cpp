/*
 * =====================================================================================
 *
 *       Filename:  Fixes.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年02月10日 00時48分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "Fixes.h"

double Fixes::getLongitudeFromDMS(std::string long_dms)
{
	std::stringstream ss("");
	double degree,min,sec;
	ss<<long_dms[0]<<long_dms[1]<<long_dms[2]<<'\t';
	ss<<long_dms[3]<<long_dms[4]<<'\t';
	ss<<long_dms[5]<<long_dms[6]<<long_dms[7]<<long_dms[8]<<long_dms[9];
	
	ss >> degree;
	ss >> min;
	ss >> sec;
	
	return degree + min / 60.0 + sec / 3600.0;
}
double Fixes::getLatitudeFromDMS(std::string lat_dms)
{
	std::stringstream ss("");
	double degree,min,sec;
	ss<<lat_dms[0]<<lat_dms[1]<<'\t';
	ss<<lat_dms[2]<<lat_dms[3]<<'\t';
	ss<<lat_dms[4]<<lat_dms[5]<<lat_dms[6]<<lat_dms[7]<<lat_dms[8];
	
	ss >> degree;
	ss >> min;
	ss >> sec;
	
	return degree + min / 60.0 + sec / 3600.0;

}


void Fixes::init(DBAccessor& dba)
{
	//Fixを取得するクエリを作成
	std::string sql("select fixName,longitudeDMS,latitudeDMS from Fix;");
	//クエリを実行
	dba.setQuery(sql);

	//クエリの結果を取得
	while(SQLITE_ROW == dba.step_select())
	{
		//Fix名
		std::string fix_name(dba.getColumnString(0));
		//経度(DMS)
		std::string longitudeDMS(dba.getColumnString(1));
		//緯度(DMS)
		std::string latitudeDMS(dba.getColumnString(2));
		//経度(double)
		double longitude = getLongitudeFromDMS(longitudeDMS);
		//緯度(double)
		double latitude = getLatitudeFromDMS(latitudeDMS);
		//vectorに追加
		this->name.push_back(fix_name);
		this->longitude.push_back(longitude);
		this->latitude.push_back(latitude);
	}
}
