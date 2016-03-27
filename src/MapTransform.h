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
#ifndef MapTransform_H
#define MapTransform_H

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
	void setTransform()
	{
		//モデリング変換行列の設定
		//モデル座標は経緯度-feetのサイズになっているので
		//地図っぽくスケーリングする
		//経緯度はそのままで行けそうなので、feetのみ変更
		//40000feetが4cmになるように1/10000に縮小する
		glScaled(scale,scale,0.0001);
		//羽田+offsetがワールド座標系の原点に来るように平行移動する
		double haneda_x = 139.0 + ( 46.0/60.0 + 87.0/3600.0 );
		double haneda_y =  35.0 + ( 33.0/60.0 + 20.0/3600.0 );
		glTranslated( - haneda_x - centerOffsetLong, - haneda_y - centerOffsetLat, 0.0);
	}
};












#endif
