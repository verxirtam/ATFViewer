/*
 * =====================================================================================
 *
 *       Filename:  Util.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年02月14日 02時38分12秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef UTIL_H_
#define UTIL_H_

#include <sstream>

class Util
{
public:
	static double getLongitudeFromDMS(std::string long_dms)
	{
		std::stringstream ss("");
		double degree,min,sec;
		ss<<long_dms[0]<<long_dms[1]<<long_dms[2]<<'\t';
		ss<<long_dms[3]<<long_dms[4]<<'\t';
		ss<<long_dms[5]<<long_dms[6]<<long_dms[7]<<long_dms[8]<<long_dms[9];
		
		double sign = 1.0;
		switch (long_dms[10])
		{
			case 'E':
				 sign = 1.0;
				 break;
			case 'W':
				 sign = -1.0;
				break;
		}
		
		ss >> degree;
		ss >> min;
		ss >> sec;
		
		return sign * ( degree + min / 60.0 + sec / 3600.0 );
	}
	static double getLatitudeFromDMS(std::string lat_dms)
	{
		std::stringstream ss("");
		double degree,min,sec;
		ss<<lat_dms[0]<<lat_dms[1]<<'\t';
		ss<<lat_dms[2]<<lat_dms[3]<<'\t';
		ss<<lat_dms[4]<<lat_dms[5]<<lat_dms[6]<<lat_dms[7]<<lat_dms[8];
	
		double sign = 1.0;
		switch (lat_dms[9])
		{
			case 'N':
				 sign = 1.0;
				 break;
			case 'S':
				 sign = -1.0;
				break;
		}
		ss >> degree;
		ss >> min;
		ss >> sec;
		
		return sign * ( degree + min / 60.0 + sec / 3600.0 );
	}
	static int getFeetFromFL(std::string fl)
	{
		std::stringstream ss("");
		ss << fl[2] << fl[3] << fl[4];
		int feet;
		ss >> feet;
		return feet * 100;
	}
};

#endif
