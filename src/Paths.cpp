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
	
	//軌道を取得する
	std::stringstream sql("");
	//sql<<"select id,longitude,latitude,altitude,time from TrackData where time>=";
	sql<<"select id,longitude,latitude,altitude,time,arrival from TrackData where time>=";
	sql<<time_min<<" and time<";
	sql<<time_max<<" order by id,time;";
	dba.setQuery(sql.str());
	cout<<"setQuery() after, step_select() before"<<endl;
	
	//パスのインデックス
	int n=-1;
	//直前に読み込んだid
	//id毎にvectorに格納するため
	std::string old_id("");
	
	while(SQLITE_ROW == dba.step_select())
	{
		std::string id(dba.getColumnString(0));
		double lo=dba.getColumnDouble(1);
		double la=dba.getColumnDouble(2);
		int a=dba.getColumnInt(3);
		long long t=dba.getColumnLongLong(4);
		std::string ar(dba.getColumnString(5));
		if(id != old_id)
		{
			n++;
			paths.push_back(vector<PathPoint>());
			past_time_index.push_back(0);
			now_index.push_back(0);
			old_id = id;
		}
		paths[n].push_back(PathPoint(lo,la,a,t,ar));
	}
	cout<<"initPathPoint() end"<<endl;
}

void Paths::resetTime(void)
{
	int nmax = now_index.size();
	for(int n = 0; n < nmax; n++)
	{
		past_time_index[n] = 0;
		now_index[n] = 0;
	}
}

void Paths::display(time_t now)
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
	//航空機毎に軌道を描画する
	for (unsigned int n = 0; n < paths.size(); n++)
	{
		//未来の軌道は描かない
		if (paths[n][0].time > now )
		{
			continue;
		}
		glBegin(GL_TRIANGLE_STRIP);
		//航空機の軌道に垂線をおろした帯状の図形を描画する
		int path_size=paths[n].size();
		//past_time_indexを更新する
		//past_time_index[n] = Min{ i | past_time < paths[n][i].time}
		for(int i = past_time_index[n]; i < path_size; i++)
		{
			if(past_time < paths[n][i].time)
			{
				past_time_index[n] = i;
				break;
			}
		}
		//past_timeが軌道のどの時刻よりも大きい場合はpath_size - 1を設定する
		if(paths[n][path_size-1].time <= past_time)
		{
			past_time_index[n] = path_size;
		}
		//now_indexを更新する
		//now_index[n] = Min{ i | now < paths[n][i].time}
		for(int i = now_index[n]; i < path_size; i++)
		{
			if(now < paths[n][i].time)
			{
				now_index[n] = i;
				break;
			}
		}
		//nowが軌道のどの時刻よりも大きい場合はpath_size - 1を設定する
		if(paths[n][path_size-1].time <= now)
		{
			now_index[n] = path_size;
		}
		//past_pointを描画する
		//past_time_indexがpaths[n]の両端のときは対象外の時刻なので描画しない
		if((past_time_index[n] != 0) && (past_time_index[n] != path_size))
		{
			int i = past_time_index[n];
			PathPoint past_point = getNowPoint(paths[n][i-1],paths[n][i], past_time);
			drawPath(past_point,now);
		}
		//past_time_index <= i < now_indexとなるiのpath[n][i]を描画する
		//path_sizeに達したら描画を終了する
		for(int i = past_time_index[n]; i < now_index[n]; i++)
		{
			//軌道を描画する
			drawPath(paths[n][i],now);
		}
		//now_indexがpaths[n]の両端のときは対象外の時刻なので描画しない
		//now_pointを描画する
		if((now_index[n] != 0) && (now_index[n] != path_size))
		{
			int i = now_index[n];
			PathPoint now_point = getNowPoint(paths[n][i-1],paths[n][i],now);
			drawPath(now_point,now);
		}
		/////////////////////////////////////////////////////////////////////////////
		glEnd();
	}
	//アルファブレンド無効化
	glDisable(GL_BLEND);
	//デプスバッファ有効化
	glEnable(GL_DEPTH_TEST);
	//航空機の軌道の描画完了

}
