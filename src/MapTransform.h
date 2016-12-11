/*
 * =====================================================================================
 *
 *       Filename:  MapTransform.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年03月25日 09時19分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>


class MapTransform
{
private:
	double centerOffsetLong;
	double centerOffsetLat;
	double scale;
public:
	MapTransform():
		centerOffsetLong(0.0),
		centerOffsetLat(0.0),
		scale(1.0)
	{
	}
	void setCenterOffsetLong(GLdouble c_long)
	{
		centerOffsetLong = (c_long < -180.0)?(-180.0):
					((c_long > 180.0)?180.0:c_long);
	}
	GLdouble getCenterOffsetLong(void)
	{
		return centerOffsetLong;
	}
	void setCenterOffsetLat(GLdouble c_lat)
	{
		centerOffsetLat = (c_lat < -90.0)?(-90.0):
					((c_lat > 90.0)?90.0:c_lat);
	}
	GLdouble getCenterOffsetLat(void)
	{
		return centerOffsetLat;
	}
	void setScale(GLdouble s)
	{
		scale = (s > 0.0) ? s : 1.0;
	}
	GLdouble getScale(void)
	{
		return scale;
	}
	/// @brief モデリング変換行列を設定する
	///
	/// @param m 設定する行列
	void setTransform(glm::mat4& m)
	{
		
		//モデリング変換行列の設定
		//モデル座標は経緯度-feetのサイズになっているので
		//地図っぽくスケーリングする
		//経緯度はそのままで行けそうなので、feetのみ変更
		//40000feetが4cmになるように1/10000に縮小する
		m = glm::scale(glm::vec3(scale, scale, 0.0001));
		//羽田+offsetがワールド座標系の原点に来るように平行移動する
		float haneda_x = 139.0 + ( 46.0/60.0 + 87.0/3600.0 );
		float haneda_y =  35.0 + ( 33.0/60.0 + 20.0/3600.0 );
		m = m * glm::translate(glm::vec3( - haneda_x - centerOffsetLong, - haneda_y - centerOffsetLat, 0.0));
	}
};












