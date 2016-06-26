/*
 * =====================================================================================
 *
 *       Filename:  Paths.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年03月27日 21時53分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Paths.h"

using namespace std;

void Paths::drawPath(PathPoint& p, time_t now)
{
	/*
	//航空機の高度に応じて色を設定する
	//帯の地面に接する箇所はアルファを0にする（完全に透明にする）
	double c=((double)p.altitude)/40000.0;
	double alpha = 1.0 - (((double)(now - p.time)) / ((double)drawTimeWidth));
	c=c*c;
	//glColor4d(c, 0.5, 1.0 - c, alpha * (1.0 - c));
	glColor4d(c, 0.5, 1.0 - c, alpha * 0.5);
	glVertex3d(p.longitude, p.latitude, p.altitude);
	glColor4d(c, 0.5, 1.0 - c, alpha * 0.1);
	glVertex3d(p.longitude, p.latitude, 0.0);
	*/
	
	//目的空港の文字列に応じて配色する
	double alpha = 1.0 - (((double)(now - p.time)) / ((double)drawTimeWidth));
	
	int a0 = p.arrival[0] - 65;
	int a1 = p.arrival[1] - 65;
	int a2 = p.arrival[2] - 65;
	int h = a2 * 29 * 29 + a1 * 29 + a0;

	double r = ((double)(h % 251)) / 251.0;
	double g = ((double)((h * h) % 251)) / 251.0;
	double b = ((double)((h * h * h) % 251)) / 251.0;
	
	
	/*
	int r=0, g=0, b=0;
	if(p.arrival=="HND")
	{

	}
	else
	{
		r = ((double)p.altitude)/40000.0;
	}
	*/
	glColor4d(r, g, b, alpha * 0.75);
	glVertex3d(p.longitude, p.latitude, p.altitude);
	glColor4d(r, g, b, alpha * 0.1);
	glVertex3d(p.longitude, p.latitude, 0.0);
}
PathPoint Paths::getNowPoint(PathPoint& from, PathPoint& to, time_t time)
{
	PathPoint ret(0.0, 0.0, 0, 0, from.arrival);
	double ratio_from = 1.0 - ((double)(time - from.time))/((double)(to.time - from.time));
	double ratio_to = 1.0 - ratio_from;
	ret.longitude = ratio_from * from.longitude + ratio_to * to.longitude;
	ret.latitude = ratio_from * from.latitude + ratio_to * to.latitude;
	ret.altitude =(int)( ratio_from * ((double)from.altitude) + ratio_to * ((double)to.altitude));
	ret.time = time;
	return ret;
}

void Paths::initPathPoint(DBAccessor& dba, time_t time_min, time_t time_max)
{
	TrackDataManager tdm;
	tdm.getTrackDataFromDBParallel(paths,time_min,time_max);
}

void Paths::resetTime(void)
{
	int nmax = paths.size();
	for(int n = 0; n < nmax; n++)
	{
		paths[n].past_time_index = 0;
		paths[n].now_index = 0;
	}
}

void Paths::updatePastTimeIndex(Path& p, time_t past_time)
{
	int path_size = p.pathPoint.size();
	//past_time_indexを更新する
	//past_time_index = Min{ i | past_time < p[i].time}
	for(int i = p.past_time_index; i < path_size; i++)
	{
		if(past_time < p.pathPoint[i].time)
		{
			p.past_time_index = i;
			break;
		}
	}
	//past_timeが軌道のどの時刻よりも大きい場合はpath_sizeを設定する
	if(p.pathPoint[path_size-1].time <= past_time)
	{
		p.past_time_index = path_size;
	}
}

void Paths::updateNowIndex(Path& p, time_t now)
{
	int path_size = p.pathPoint.size();
	
	//now_indexを更新する
	//now_index[n] = Min{ i | now < paths[n][i].time}
	for(int i = p.now_index; i < path_size; i++)
	{
		if(now < p.pathPoint[i].time)
		{
			p.now_index = i;
			break;
		}
	}
	//nowが軌道のどの時刻よりも大きい場合はpath_sizeを設定する
	if(p.pathPoint[path_size-1].time <= now)
	{
		p.now_index = path_size;
	}
}

void Paths::drawPathLine(Path& p, time_t past_time, time_t now)
{
	int path_size = p.pathPoint.size();
	//past_pointを描画する
	glBegin(GL_TRIANGLE_STRIP);
	//past_time_indexがpの両端のときは対象外の時刻なので描画しない
	if((p.past_time_index != 0) && (p.past_time_index != path_size))
	{
		int i = p.past_time_index;
		PathPoint past_point = getNowPoint(p.pathPoint[i-1], p.pathPoint[i], past_time);
		drawPath(past_point,now);
	}
	//past_time_index <= i < now_indexとなるiのpath[n][i]を描画する
	//path_sizeに達したら描画を終了する
	for(int i = p.past_time_index; i < p.now_index; i++)
	{
		//軌道を描画する
		drawPath(p.pathPoint[i],now);
	}
	//now_indexがpaths[n]の両端のときは対象外の時刻なので描画しない
	//now_pointを描画する
	if((p.now_index != 0) && (p.now_index != path_size))
	{
		int i = p.now_index;
		PathPoint now_point = getNowPoint(p.pathPoint[i-1],p.pathPoint[i],now);
		drawPath(now_point,now);
	}
	glEnd();
}

int Paths::display(time_t now)
{
	//航空機の軌道っぽいものを描く
	//隠面消去を無効にする
	//アルファブレンドで奥の透明オブジェクトが描画されないことがあるため
	//また、軌道の手前に描かれるオブジェクトがないため最後に描画すれば前後関係は崩れない
	glDisable(GL_DEPTH_TEST);
	//アルファブレンド有効化
	glEnable(GL_BLEND);
	//アルファブレンドの方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//描画を始める時刻
	time_t past_time = now - drawTimeWidth;
	
	//描画した航空機数
	int ret = 0;
	
	//航空機毎に軌道を描画する
	for (unsigned int n = 0; n < paths.size(); n++)
	{
		Path& p = paths[n];
		
		//past_timeより過去の軌道は描かない
		if (p.pathPoint.rbegin()->time < past_time )
		{
			continue;
		}
		
		//未来の軌道は描かない
		if (p.pathPoint[0].time > now )
		{
			continue;
		}
		
		//past_time_indexを更新する
		this->updatePastTimeIndex(p, past_time);
		
		//now_indexを更新する
		this->updateNowIndex(p, now);
		
		//航空機の軌道に垂線をおろした帯状の図形を描画する
		this->drawPathLine(p, past_time, now);
		
		//描画した機数をカウント
		ret++;
	}
	//アルファブレンド無効化
	glDisable(GL_BLEND);
	//デプスバッファ有効化
	glEnable(GL_DEPTH_TEST);
	//航空機の軌道の描画完了
	
	return ret;
}
