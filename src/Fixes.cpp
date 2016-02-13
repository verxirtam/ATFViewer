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
		double longitude = Util::getLongitudeFromDMS(longitudeDMS);
		//緯度(double)
		double latitude = Util::getLatitudeFromDMS(latitudeDMS);
		//vectorに追加
		this->name.push_back(fix_name);
		this->longitude.push_back(longitude);
		this->latitude.push_back(latitude);
	}
}
