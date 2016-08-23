/*
 * =====================================================================================
 *
 *       Filename:  ATFViewerMain.h
 *
 *    Description:  メインのクラス
 *					OpenGLの設定を保持し、画面の制御を行う。
 *					シングルトンとして定義する。
 *					OpenGLの管理元を一意にするため。
 *        Version:  1.0
 *        Created:  2016年02月01日 22時25分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef ATFViewerMain_H
#define ATFViewerMain_H

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cmath>
#include <ctime>


#include "OpenGLHeaders.h"

#include "DBAccessor.h"
#include "BitmapString.h"
#include "Paths.h"
#include "MapTransform.h"
#include "Map.h"
#include "Fixes.h"
#include "Sectors.h"
#include "Joystick.h"
#include "TestVBO.h"

class ATFViewerMain
{
private:
	const double PI;
	int windowWidth;
	int windowHeight;
	const unsigned int pollingInterval;
	Joystick joystick;
	const int joystickTimerId;
	const int timeInterval;
	int currentTimeInterval;
	//デバッグ用 ここから
	unsigned int disp_buttonMask;
	int disp_X;
	int disp_Y;
	int disp_Z;
	TestVBO tv;
	//デバッグ用 ここまで
	GLdouble camera_r;
	GLdouble camera_theta;
	GLdouble camera_phi;
	GLdouble camera_target[3];
	Paths paths;
	time_t now;
	const time_t timeMin;
	const time_t timeMax;
	MapTransform mapTransform;
	std::vector<Map> map;
	Fixes fixes;
	Sectors sectors;
	//初期化
	ATFViewerMain():
		PI(3.14159265358979),
		windowWidth(100),
		windowHeight(100),
		pollingInterval(10),
		joystick(),
		joystickTimerId(1),
		timeInterval(20),//5),
		currentTimeInterval(timeInterval),
		disp_buttonMask(),
		disp_X(),
		disp_Y(),
		disp_Z(),
		tv(),
		camera_r(30.0),
		camera_theta(270.0*PI/180.0),
		camera_phi(60.0*PI/180.0),
		paths(),
		//paths_first_index(),
		//past_time_index(),
		//now_index(),
		//now(1460635200),//滑走路閉鎖のあった日(2016/4/17)
		//now(1460821854),
		//now(1453260000),
		//now(1471618800),//2016/08/20
		//now(1471705200),//2016/08/21
		now(1471273200),//2016/08/16
		timeMin(now),
		timeMax(now+60*60*24*5),//1453300000),
		mapTransform(),
		map(),
		fixes(),
		sectors()
	{
		camera_target[0]=0.0;
		camera_target[1]=0.0;
		camera_target[2]=0.0;
		//map.push_back(Map("bm_200406"));
		map.push_back(Map("ENRC1_20160204"));
		map.push_back(Map("ENRC2_20160204"));
	}
	//シーンの初期化
	void initScene(void);
	//void initPathPoint(DBAccessor& dba);
	//void drawPath(PathPoint& p);
	//PathPoint getNowPoint(PathPoint& from, PathPoint& to, double time);
	//シングルトンとするためコピーコンストラクタ、代入演算子は定義しない
	ATFViewerMain(const ATFViewerMain& a);
	ATFViewerMain& operator=(const ATFViewerMain& a);
	//画面更新用の関数
	static void _display(void)
	{
		ATFViewerMain::getInstance().display();
	}
	//画面のリサイズ時に実行される関数
	static void _resize(int w, int h)
	{
		ATFViewerMain::getInstance().resize(w,h);
	}
	//キーボード入力時に実行される関数
	static void _keyboard(unsigned char key, int x, int y)
	{
		ATFViewerMain::getInstance().keyboard(key,x,y);
	}
	//ジョイスティックイベントの検出のための関数
	static void _joystickTimer(int value)
	{
		ATFViewerMain::getInstance().joystickTimer(value);
	}
	static void _idle(void)
	{
		ATFViewerMain::getInstance().idle();
	}
	//画面更新用の関数
	void display(void);
	//画面のリサイズ時に実行される関数
	void resize(int w, int h);
	//キーボード入力時に実行される関数
	void keyboard(unsigned char key, int x, int y);
	//ジョイスティックイベントの検出のためのタイマー関数
	void joystickTimer(int value);
	void idle(void)
	{
		glutPostRedisplay();
	}
public:
	//インスタンスの取得
	inline static ATFViewerMain& getInstance()
	{
		static ATFViewerMain atfvm;
		return atfvm;
	}
	void init(void)
	{
		std::cout << "ATFViewerMain::init()" << std::endl;
		
		int argc=0;
		char* argv[]={};
		//GLUTの初期化
		glutInit(&argc,argv);
		std::cout << "glutInit(&argc,argv) end." << std::endl;
		//ディスプレイモードの設定
		glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
		//ウィンドウの生成
		glutCreateWindow("ATFViewer");
		GLenum err;
		err = glewInit();
		if(err != GLEW_OK)
		{
			std::cout << "error at glewInit()." << std::endl;
		}
		//画面更新関数の設定
		glutDisplayFunc(ATFViewerMain::_display);
		//画面リサイズ時に実行される関数の設定
		glutReshapeFunc(ATFViewerMain::_resize);
		//キーボード入力時に実行される関数の設定
		glutKeyboardFunc(ATFViewerMain::_keyboard);
		//ジョイスティックイベントの検出のための関数
		glutTimerFunc(pollingInterval, ATFViewerMain::_joystickTimer, joystickTimerId);
		//アイドル時に実行される関数の設定
		glutIdleFunc(ATFViewerMain::_idle);
		//シーンの初期化
		this->initScene();
	}
	void execMainLoop(void)
	{
		glutMainLoop();
	}
};








#endif

