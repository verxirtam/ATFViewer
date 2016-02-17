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

#include <GL/glut.h>

#include "DBAccessor.h"
#include "Map.h"
#include "Fixes.h"

struct PathPoint
{
	double longitude;
	double latitude;
	int altitude;
	time_t time;
	PathPoint(double lo,double la, int a, long long t)
	{
		longitude=lo;
		latitude=la;
		altitude=a;
		time=t;
	}
};



class ATFViewerMain
{
private:
	const double PI;
	GLdouble camera_r;
	GLdouble camera_theta;
	GLdouble camera_phi;
	GLdouble camera_target[3];
	std::vector<std::vector<PathPoint> > paths;
	std::vector<int> paths_first_index;
	std::vector<int> past_time_index;
	std::vector<int> now_index;
	time_t now;
	const time_t timeMin;
	const time_t timeMax;
	Map map;
	Fixes fixes;
	//初期化
	ATFViewerMain():
		PI(3.14159265358979),
		camera_r(30.0),
		camera_theta(300.0*PI/180.0),
		camera_phi(60.0*PI/180.0),
		paths(),
		paths_first_index(),
		past_time_index(),
		now_index(),
		now(1453260000),
		timeMin(now),
		timeMax(now+60*60*24),//1453300000),
		map(),
		fixes()
	{
		camera_target[0]=0.0;
		camera_target[1]=0.0;
		camera_target[2]=0.0;
	}
	//シーンの初期化
	void initScene(void);
	void initPathPoint(DBAccessor& dba);
	void drawPath(PathPoint& p);
	PathPoint getNowPoint(PathPoint& from, PathPoint& to, double time);
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
		int argc=0;
		char* argv[]={};
		//GLUTの初期化
		glutInit(&argc,argv);
		//ディスプレイモードの設定
		glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
		//ウィンドウの生成
		glutCreateWindow("ATFViewer");
		//画面更新関数の設定
		glutDisplayFunc(ATFViewerMain::_display);
		//画面リサイズ時に実行される関数の設定
		glutReshapeFunc(ATFViewerMain::_resize);
		//キーボード入力時に実行される関数の設定
		glutKeyboardFunc(ATFViewerMain::_keyboard);
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

