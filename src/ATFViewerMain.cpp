/*
 * =====================================================================================
 *
 *       Filename:  ATFViewerMain.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年02月02日 00時53分02秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "ATFViewerMain.h"

using namespace std;

void ATFViewerMain::drawPath(PathPoint& p)
{
	//航空機の高度に応じて色を設定する
	//帯の地面に接する箇所はアルファを0にする（完全に透明にする）
	double c=((double)p.altitude)/40000.0;
	double alpha = 1.0 - (((double)(now - p.time)) / 600.0);
	c=c*c;
	//glColor4d(c, 0.5, 1.0 - c, alpha * (1.0 - c));
	glColor4d(c, 0.5, 1.0 - c, alpha * 0.5);
	glVertex3d(p.longitude, p.latitude, p.altitude);
	glColor4d(c, 0.5, 1.0 - c, alpha * 0.1);
	glVertex3d(p.longitude, p.latitude, 0.0);
}
PathPoint ATFViewerMain::getNowPoint(PathPoint& from, PathPoint& to, double time)
{
	PathPoint ret(0.0, 0.0, 0, 0);
	double ratio_from = 1.0 - ((double)(time - from.time))/((double)(to.time - from.time));
	double ratio_to = 1.0 - ratio_from;
	ret.longitude = ratio_from * from.longitude + ratio_to * to.longitude;
	ret.latitude = ratio_from * from.latitude + ratio_to * to.latitude;
	ret.altitude =(int)( ratio_from * ((double)from.altitude) + ratio_to * ((double)to.altitude));
	ret.time = time;
	return ret;
}

void ATFViewerMain::initPathPoint(DBAccessor& dba)
{
	
	//軌道を取得する
	std::stringstream sql("");
	sql<<"select id,longitude,latitude,altitude,time from TrackData where time>=";
	sql<<timeMin<<" and time<";
	sql<<timeMax<<" order by id,time;";
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
		if(id != old_id)
		{
			n++;
			paths.push_back(vector<PathPoint>());
			paths_first_index.push_back(0);
			old_id = id;
		}
		paths[n].push_back(PathPoint(lo,la,a,t));
	}
	cout<<"initPathPoint() end"<<endl;
}


void ATFViewerMain::initScene(void)
{
	cout<<"dba() before"<<endl;
	DBAccessor dba(std::string("../../db/ATFViewer.db"));
	cout<<"dba() after, setQuery() before"<<endl;
	
	
	initPathPoint(dba);
	
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//フィックスの初期化
	fixes.init(dba);
	
	//マップの初期化
	map.init(dba);
	
	//デプスバッファを使用する
	glEnable(GL_DEPTH_TEST);
}


void ATFViewerMain::display(void)
{
	//時刻の更新
	now+=20;
	if (now > timeMax)
	{
		now = timeMin;
		int nmax = paths_first_index.size();
		for(int n = 0; n < nmax; n++)
		{
			paths_first_index[n]=0;
		}
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//モデルビュー変換行列の初期化
	glLoadIdentity();
	
	//ビュー変換行列の設定
	//カメラの位置、向きの設定
	double cx=camera_target[0] + camera_r * cos(camera_phi) * cos(camera_theta);
	double cy=camera_target[1] + camera_r * cos(camera_phi) * sin(camera_theta);
	double cz=camera_target[2] + camera_r * sin(camera_phi);
	gluLookAt(
			cx,cy,cz,//カメラの位置
			camera_target[0],camera_target[1],camera_target[2],//視点の位置
			0.0,   0.0,  1.0 //カメラから見た垂直方向
			);
	
	//光源の設定
	//const static GLfloat light0pos[] = { 0.0, 0.0, 100.0, 1.0 };
	//glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	
	
	//クリッピング平面を設定
	double cp[][4]=
		{
			{-1.0, 0.0, 0.0, 20.0},
			{ 1.0, 0.0, 0.0, 20.0},
			{ 0.0,-1.0, 0.0, 20.0},
			{ 0.0, 1.0, 0.0, 20.0}
		};
	glClipPlane(GL_CLIP_PLANE0,cp[0]);
	glClipPlane(GL_CLIP_PLANE1,cp[1]);
	glClipPlane(GL_CLIP_PLANE2,cp[2]);
	glClipPlane(GL_CLIP_PLANE3,cp[3]);
	//クリッピング平面の設定
	glEnable(GL_CLIP_PLANE0);
	glEnable(GL_CLIP_PLANE1);
	glEnable(GL_CLIP_PLANE2);
	glEnable(GL_CLIP_PLANE3);
	
	//マップの描画
	map.display();
	//以下はmap座標系（緯度経度座標系）

	//フィックスを描画する
	fixes.display();

	//航空機の軌道っぽいものを描く
	//隠面消去を無効にする
	//アルファブレンドで奥の透明オブジェクトが描画されないことがあるため
	//また、軌道の手前に描かれるオブジェクトがないため最後に描画すれば前後関係は崩れない
	glDisable(GL_DEPTH_TEST);
	//アルファブレンド有効化
	glEnable(GL_BLEND);
	//アルファブレンドの方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
		int imin = paths_first_index[n];
		///////////////////////////////////////////////////
		/*
		for (int i = imin; i < path_size-1; i++)
		{
			if(paths[n][i].time > now)
			{
				PathPoint now_point = getNowPoint(paths[n][i-1],paths[n][i],now);
				drawPath(now_point);
				break;
			}
			else
			{
				if(paths[n][i].time >= (now - 600) )
				{
					drawPath(paths[n][i]);
				}
				else
				{
					paths_first_index[n]=i;
				}
			}
		}
		*/
		////////////////////////////////////////////////////
		int i = 0;
		long long past_time = now - 600;
		//past_timeより前の軌道は描かない
		for (i = imin; i < path_size; i++)
		{
			if( past_time <= paths[n][i].time )
			{
				break;
			}
		}
		if (i == path_size)
		{
			//描画する軌道なし
		}
		else
		{
			//past_timeの直前のインデックスを保存
			paths_first_index[n] = i - 1;
			//past_timeの軌道を描画
			PathPoint past_point = getNowPoint(paths[n][i-1],paths[n][i], past_time);
			drawPath(past_point);
			//past_time以降の軌道を描画
			for (; i < path_size; i++)
			{
				if(paths[n][i].time > now)
				{
					//nowの直後の点が現れたらnowの点として内分点を描画する
					PathPoint now_point = getNowPoint(paths[n][i-1],paths[n][i],now);
					drawPath(now_point);
					break;
				}
				else
				{
					//軌道を描画する
					drawPath(paths[n][i]);
				}
			}
		}
		////////////////////////////////////////////////////
		glEnd();
	}
	//アルファブレンド無効化
	glDisable(GL_BLEND);
	//デプスバッファ有効化
	glEnable(GL_DEPTH_TEST);
	//航空機の軌道の描画完了

	//クリッピング平面の無効化
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glDisable(GL_CLIP_PLANE2);
	glDisable(GL_CLIP_PLANE3);
	
	//オブジェクト描画コマンドを発行する
	//glFlush();
	//描画対象のバッファを入れ替える
	glutSwapBuffers();
}

void ATFViewerMain::resize(int w, int h)
{
	//ウィンドウのどの範囲をビューポートにするかを設定する
	//下記はウィンドウの全範囲をビューポートに設定している
	glViewport(0, 0, w, h);

	//変換行列の初期化
	//透視変換行列の設定
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, ((double)w)/(double)h, 1.0, 1000.0);

	//モデルビュー変換行列の設定
	//モデリング変換（モデル- -> ワールド）、
	//ビューイング変換(ワールド -> カメラ)
	//を行うための設定
	glMatrixMode(GL_MODELVIEW);

}

void ATFViewerMain::keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'f':
		//center_offset_long += 1.0 / scale;
		map.setCenterOffsetLong(map.getCenterOffsetLong() + 1.0 / map.getScale());
		display();
		break;
	case 's':
		//center_offset_long -= 1.0 / scale;
		map.setCenterOffsetLong(map.getCenterOffsetLong() - 1.0 / map.getScale());
		display();
		break;
	case 'e':
		//center_offset_lat += 1.0 / scale;
		map.setCenterOffsetLat(map.getCenterOffsetLat() + 1.0 / map.getScale());
		display();
		break;
	case 'd':
		//center_offset_lat -= 1.0 / scale;
		map.setCenterOffsetLat(map.getCenterOffsetLat() - 1.0 / map.getScale());
		display();
		break;
	case 'l':
		camera_theta+=10.0*PI/180.0;
		camera_theta = (camera_theta > 2.0 * PI) ? camera_theta-2.0*PI : camera_theta;
		display();
		break;
	case 'j':
		camera_theta-=10.0*PI/180.0;
		camera_theta = (camera_theta < 0.0) ? camera_theta+2.0*PI : camera_theta;
		display();
		break;
	case 'i':
		camera_phi+=5.0*PI/180.0;
		camera_phi = (camera_phi >80.0*PI/180.0) ? 80.0*PI/180.0 : camera_phi;
		display();
		break;
	case 'k':
		camera_phi-=5.0*PI/180.0;
		camera_phi = (camera_phi < 0.0) ? 0.0*PI/180.0 : camera_phi;
		display();
		break;
	case 'b':
		//scale*=0.875;
		map.setScale(map.getScale()*0.875);
		display();
		break;
	case ' ':
		//scale/=0.875;
		map.setScale(map.getScale()/0.875);
		display();
		break;
	case 't':
		static bool animation_enable = true;
		if(animation_enable)
		{
			glutIdleFunc(ATFViewerMain::_idle);
			animation_enable = false;
		}
		else
		{
			glutIdleFunc(NULL);
			animation_enable = true;
		}
		break;
	}
}




