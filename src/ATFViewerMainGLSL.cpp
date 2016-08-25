/*
 * =====================================================================================
 *
 *       Filename:  ATFViewerMainGLSL.cpp
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
#include "ATFViewerMainGLSL.h"

using namespace std;


void ATFViewerMainGLSL::initScene(void)
{
	cout<<"initScene() start"<<endl;
	
	textureShaderProgram.init();
	basicShaderProgram.init();
	
	DBAccessor dba("../../db/ATFViewer.db");
	map.init(dba);
	map2.init(dba);
	sector.init(dba);
	path.initPathPoint(this->timeMin, this->timeMax);

	cout<<"initScene() end"<<endl;
}

void ATFViewerMainGLSL::setMatrix(void)
{
	glm::mat4 projection = glm::perspective
		(
			3.141592f * 30.0f / 180.0f,
			static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
			1.0f,
			1000.f
		);
	
	glm::vec3 camera_state
		(
			camera_target[0] + camera_r * cos(camera_phi) * cos(camera_theta),
			camera_target[1] + camera_r * cos(camera_phi) * sin(camera_theta),
			camera_target[2] + camera_r * sin(camera_phi)
		);
	glm::mat4 view = glm::lookAt
		(
			camera_state,//カメラの位置
			glm::vec3(camera_target[0], camera_target[1],  camera_target[2]),//視点の位置
			glm::vec3(  0.0f,  0.0f,  1.0f) //カメラの上方向の向き
		);
	glm::mat4 model(1.0);
	mapTransform.setTransform(model);
	
	glm::mat4 mvp(projection * view * model);
	
	textureShaderProgram.setMVPMatrix(mvp);
	basicShaderProgram.setMVPMatrix(mvp);
}

void ATFViewerMainGLSL::display(void)
{
	//時刻の更新
	now += currentTimeInterval;
	if (now > timeMax)
	{
		now = timeMin;
		path.resetTime();
	}
	
	//フレームバッファ、深度バッファをクリアする
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	setMatrix();
	
	map.display();
	map2.display();
	sector.display();
	path.display(now);
	
	//描画対象のバッファを入れ替える
	glutSwapBuffers();
}


void ATFViewerMainGLSL::resize(int w, int h)
{
	//ウィンドウのどの範囲をビューポートにするかを設定する
	//下記はウィンドウの全範囲をビューポートに設定している
	glViewport(0, 0, w, h);

	windowWidth = w;
	windowHeight = h;
}

void ATFViewerMainGLSL::keyboard(unsigned char key, int x, int y)
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
			glutIdleFunc(ATFViewerMainGLSL::_idle);
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
		//sectors.switchDisplaySector();
		break;
	}
}


//ジョイスティックイベントの検出のための関数
void ATFViewerMainGLSL::joystickTimer(int value)
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
	glutTimerFunc(pollingInterval, ATFViewerMainGLSL::_joystickTimer, joystickTimerId);
}

