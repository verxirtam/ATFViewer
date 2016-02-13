/*
 * =====================================================================================
 *
 *       Filename:  Fixes.h
 *
 *    Description:  フィックスの描画を行う
 *
 *        Version:  1.0
 *        Created:  2016年02月03日 23時52分27秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef FIXES_H_
#define FIXES_H_

#include <vector>
#include <string>
#include <sstream>

#include <GL/glut.h>

#include "Util.h"
#include "DBAccessor.h"

class Fixes
{
private:
	//フィックス名
	std::vector<std::string> name;
	//経度
	std::vector<double> longitude;
	//緯度
	std::vector<double> latitude;
public:
	Fixes()
	{
		/*
		name.push_back("haneda");
		longitude.push_back(139.0+46.0/60.0+87.0/3600.0);
		latitude.push_back(  35.0+33.0/60.0+20.0/3600.0);
		name.push_back("AKARA");
		longitude.push_back(123.0+29.0/60.0+54.0/3600.0);
		latitude.push_back(  31.0+30.0/60.0+12.0/3600.0);
		name.push_back("LAMEN");
		longitude.push_back(123.0+59.0/60.0+54.0/3600.0);
		latitude.push_back(  31.0+36.0/60.0+48.0/3600.0);
		name.push_back("SADLI");
		longitude.push_back(125.0+00.0/60.0+00.0/3600.0);
		latitude.push_back(  31.0+49.0/60.0+48.0/3600.0);
		name.push_back("NIRAT");
		longitude.push_back(126.0+03.0/60.0+29.0/3600.0);
		latitude.push_back(  32.0+03.0/60.0+54.0/3600.0);
		name.push_back("ONIKU");
		longitude.push_back(126.0+39.0/60.0+17.0/3600.0);
		latitude.push_back(  32.0+11.0/60.0+42.0/3600.0);
		name.push_back("POTET");
		longitude.push_back(127.0+02.0/60.0+23.0/3600.0);
		latitude.push_back(  32.0+16.0/60.0+48.0/3600.0);
		name.push_back("SAPRA");
		longitude.push_back(130.0+43.0/60.0+25.0/3600.0);
		latitude.push_back(  35.0+49.0/60.0+26.0/3600.0);
		*/
	}
	void init(DBAccessor& dba);
	void display()
	{
		glColor3d(0.0,0.0,1.0);
		for(unsigned int n = 0; n < longitude.size(); n++)
		{
			/*
			for (int i = 0; i < 10; i++)
			{
				double id = (double)i;
				double d = 0.03125 * i;
				glBegin(GL_LINE_LOOP);
				glColor3d(1.0,1.0,0.0);
				glVertex3d(longitude[n]-d, latitude[n]-d, 2000.0*id);
				glVertex3d(longitude[n]+d, latitude[n]-d, 2000.0*id);
				glVertex3d(longitude[n]+d, latitude[n]+d, 2000.0*id);
				glVertex3d(longitude[n]-d, latitude[n]+d, 2000.0*id);
				glEnd();
			}
			*/
			glBegin(GL_LINES);
			glVertex3d(longitude[n],latitude[n],    0.0);
			glVertex3d(longitude[n],latitude[n],50000.0);
			glEnd();
		}
	}
};



#endif
