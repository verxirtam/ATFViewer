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


void ATFViewerMain::initScene(void)
{
	cout<<"initScene() start"<<endl;
	
	//DBへの接続
	DBAccessor dba(std::string("../../db/ATFViewer.db"));
	
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	
	cout<<"\tpaths.initPathPoint() start"<<endl;
	//軌道の初期化
	paths.initPathPoint(dba, timeMin, timeMax);
	cout<<"\tpaths.initPathPoint() end"<<endl;
	
	cout<<"\tfixes.init() start"<<endl;
	//フィックスの初期化
	fixes.init(dba);
	cout<<"\tfixes.init() end"<<endl;
	
	cout<<"\tsectors.init() start"<<endl;
	//セクターの初期化
	sectors.init(dba);
	cout<<"\tsectors.init() end"<<endl;
	
	cout<<"\tmap[].init() start"<<endl;
	//マップの初期化
	int imax = map.size();
	for(int i=0;i < imax; i++)
	{
		map[i].init(dba);
	}
	cout<<"\tmap[].init() end"<<endl;

	//デプスバッファを使用する
	glEnable(GL_DEPTH_TEST);
	
	cout<<"initScene() end"<<endl;
}


void ATFViewerMain::display(void)
{
	//時刻の更新
	now += currentTimeInterval;
	if (now > timeMax)
	{
		now = timeMin;
		paths.resetTime();
	}
	
	
	static int account = 0;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//ウィンドウ座標系上での描画
	/////////////////////////////////////
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,((double)windowWidth),0.0,((double)windowHeight),-1.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//時刻を表示する
	glColor3d(1.0,1.0,1.0);
	BitmapString::drawString(0.0, (double)windowHeight-10.0,::asctime(::localtime(&now)));
	
	glColor3d(0.0,0.0,0.0);
	BitmapString::drawString(0.0, (double)windowHeight-20.0,::asctime(::localtime(&now)));
	
	//ジョイスティックの状況を表示する
	if(joystick.isEnable())
	{
		//joystick.readJoystickEvent();
		std::stringstream jss;
		jss << "Joystick: ";
		for(int i = 0; i < 6; i++)
		{
			jss << "Axis[" << i << "] = " << joystick.getAxisState(i);
		}
		for(int i = 0; i < 16; i++)
		{
			jss << "Button[" << i << "] = " << joystick.getButtonState(i);
		}
		
		
		glColor3d(1.0,1.0,1.0);
		BitmapString::drawString(0.0, (double)windowHeight-30.0, jss.str().c_str());
		glColor3d(0.0,0.0,0.0);
		BitmapString::drawString(0.0, (double)windowHeight-40.0, jss.str().c_str());
	}
	
	//描画対象の航空機数を表示する
	std::stringstream accountstr;
	accountstr << "AC Count = " << account;
	glColor3d(1.0,1.0,1.0);
	BitmapString::drawString(0.0, (double)windowHeight-50.0, accountstr.str().c_str());
	glColor3d(0.0,0.0,0.0);
	BitmapString::drawString(0.0, (double)windowHeight-60.0, accountstr.str().c_str());
	
	
	//ワールド座標系上での描画
	//////////////////////////////////////
	//変換行列の初期化
	//透視変換行列の設定
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, ((double)windowWidth)/(double)windowHeight, 1.0, 1000.0);

	//モデルビュー変換行列の設定
	//モデリング変換（モデル- -> ワールド）、
	//ビューイング変換(ワールド -> カメラ)
	//を行うための設定
	glMatrixMode(GL_MODELVIEW);
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
	
	
	//マップの座標変換を行う
	mapTransform.setTransform();
	
	//以下はmap座標系（緯度経度座標系）

	//マップの描画
	int imax = map.size();
	for(int i = 0; i < imax; i++)
	{
		map[i].display();
	}

	//フィックスを描画する
	fixes.display();

	//セクタを描画する
	sectors.display();


	//航空機の軌道を描画する
	account = paths.display(now);

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

	windowWidth = w;
	windowHeight = h;
}

void ATFViewerMain::keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'f':
		//center_offset_long += 1.0 / scale;
		mapTransform.setCenterOffsetLong(mapTransform.getCenterOffsetLong() + 1.0 / mapTransform.getScale());
		display();
		break;
	case 's':
		//center_offset_long -= 1.0 / scale;
		mapTransform.setCenterOffsetLong(mapTransform.getCenterOffsetLong() - 1.0 / mapTransform.getScale());
		display();
		break;
	case 'e':
		//center_offset_lat += 1.0 / scale;
		mapTransform.setCenterOffsetLat(mapTransform.getCenterOffsetLat() + 1.0 / mapTransform.getScale());
		display();
		break;
	case 'd':
		//center_offset_lat -= 1.0 / scale;
		mapTransform.setCenterOffsetLat(mapTransform.getCenterOffsetLat() - 1.0 / mapTransform.getScale());
		display();
		break;
	case 'l':
		camera_theta+=2.5*PI/180.0;
		camera_theta = (camera_theta > 2.0 * PI) ? camera_theta-2.0*PI : camera_theta;
		display();
		break;
	case 'j':
		camera_theta-=2.5*PI/180.0;
		camera_theta = (camera_theta < 0.0) ? camera_theta+2.0*PI : camera_theta;
		display();
		break;
	case 'i':
		camera_phi+=5.0*PI/180.0;
		camera_phi = (camera_phi > 90.0*PI/180.0) ? 90.0*PI/180.0 : camera_phi;
		display();
		break;
	case 'k':
		camera_phi-=5.0*PI/180.0;
		camera_phi = (camera_phi < 0.0) ? 0.0*PI/180.0 : camera_phi;
		display();
		break;
	case 'b':
		//scale*=0.875;
		mapTransform.setScale(mapTransform.getScale()*0.875);
		display();
		break;
	case ' ':
		//scale/=0.875;
		mapTransform.setScale(mapTransform.getScale()/0.875);
		display();
		break;
	case 't':
		std::cout<<::asctime(::localtime(&now))<<std::endl;
		static bool animation_enable = true;
		if(animation_enable)
		{
			glutIdleFunc(ATFViewerMain::_idle);
			animation_enable = false;
			currentTimeInterval = 0;
		}
		else
		{
			glutIdleFunc(NULL);
			animation_enable = true;
			currentTimeInterval = timeInterval;
		}
		break;
	case 'y':
		//表示するセクタを切り替える
		sectors.switchDisplaySector();
		break;
	}
}


//ジョイスティックイベントの検出のための関数
void ATFViewerMain::joystickTimer(int value)
{
	//ジョイスティックの状況を取得する
	if(joystick.isEnable())
	{
		joystick.readJoystickEvent();
	}
	
	//ポーリング間隔に応じた倍率
	double pr = ((double)(pollingInterval)) / 100.0;
	
	
	//各軸の傾きに応じた割合
	const int baffer = 3000;
	const double max = 32767.0;
	const int maxint = 32767;
	
	//ジョイスティックの操作が行われたかを表す
	bool occurred = false;
	
	//X軸:左スティック左右
	double xd = 0.0;
	int x = joystick.getAxisState(Axis_LeftStick_LeftRight);
	//Y軸:左スティック上下
	double yd = 0.0;
	int y = joystick.getAxisState(Axis_LeftStick_UpDown);
	if((x >= baffer) || (x < -baffer) || (y >= baffer) || (y < -baffer))
	{
		xd = ((double)(x)) / max * pr;
		yd = ((double)(y)) / max * pr;
		occurred = true;
	}
	
	//U軸:右スティック左右
	double ud = 0.0;
	int u = joystick.getAxisState(Axis_RightStick_LeftRight);
	//V軸:右スティック上下
	double vd = 0.0;
	int v = joystick.getAxisState(Axis_RightStick_UpDown);
	if((u >= baffer) || (u < -baffer) || (v >= baffer) || (v < -baffer))
	{
		ud = ((double)(u)) / max * pr;
		vd = ((double)(v)) / max * pr;
		occurred = true;
	}
	
	
	//マップの拡大縮小率
	double rd = 1.0;
	
	//左トリガー
	int p = joystick.getAxisState(Axis_LeftTrigger_OffOn);
	
	//右トリガー
	int q = joystick.getAxisState(Axis_RightTrigger_OffOn);
	if((p >= - maxint + baffer) || (q >= - maxint + baffer))
	{
		double pd = ((double)(p + maxint)) / (2 * max) * pr;
		double qd = ((double)(q + maxint)) / (2 * max) * pr;
		rd = 1.0 - 0.125 * (pd - qd);
		
		occurred = true;
	}
	
	
	//ジョイスティック操作が行われた時の処理
	if(occurred)
	{
		//視点の位置の移動
		//x:視点の位置(経度方向)の移動
		mapTransform.setCenterOffsetLong(mapTransform.getCenterOffsetLong() + xd / mapTransform.getScale());
		
		//y:視点の位置(緯度方向)の移動
		mapTransform.setCenterOffsetLat(mapTransform.getCenterOffsetLat() - yd / mapTransform.getScale());
		
		//カメラの移動
		//x:カメラを左右に移動
		camera_theta += 5.0 * PI / 180.0 * ud;
		camera_theta = (camera_theta > 2.0 * PI) ? camera_theta-2.0*PI : camera_theta;
		camera_theta = (camera_theta < 0.0) ? camera_theta+2.0*PI : camera_theta;
		
		//y:カメラを上下に移動
		camera_phi -= 5.0 * PI / 180.0 * vd;
		camera_phi = (camera_phi > 90.0*PI/180.0) ? 90.0*PI/180.0 : camera_phi;
		camera_phi = (camera_phi < 0.0) ? 0.0*PI/180.0 : camera_phi;
		
		//マップの拡大縮小
		mapTransform.setScale(mapTransform.getScale() * rd);
	}
	//タイマー関数の設定
	glutTimerFunc(pollingInterval, ATFViewerMain::_joystickTimer, joystickTimerId);
}

