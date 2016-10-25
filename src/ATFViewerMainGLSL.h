/*
 * =====================================================================================
 *
 *       Filename:  ATFViewerMainGLSL.h
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

#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cmath>
#include <ctime>


#include "OpenGLHeaders.h"

#include "DBAccessor.h"
#include "MapTransform.h"
#include "Joystick.h"
#include "MapVAO.h"
#include "SectorsVAO.h"
#include "PathsVAO.h"

class ATFViewerMainGLSL
{
private:
	GLFWwindow* window;
	const double PI;
	int windowWidth;
	int windowHeight;
	const unsigned int pollingInterval;
	Joystick joystick;
	const int joystickTimerId;
	const int timeInterval;
	int currentTimeInterval;
	GLdouble camera_r;
	GLdouble camera_theta;
	GLdouble camera_phi;
	GLdouble camera_target[3];
	//Paths paths;
	time_t now;
	const time_t timeMin;
	const time_t timeMax;
	MapTransform mapTransform;
	ShaderProgramTexture shaderProgramTexture;
	ShaderProgramPositionColor shaderProgramPositionColor;
	ShaderProgramPaths shaderProgramPaths;
	MapVAO map;
	MapVAO map2;
	SectorsVAO sector;
	PathsVAO path;
	//初期化
	ATFViewerMainGLSL():
		window(nullptr),
		PI(3.14159265358979),
		windowWidth(100),
		windowHeight(100),
		pollingInterval(10),
		joystick(),
		joystickTimerId(1),
		timeInterval(20),//5),
		currentTimeInterval(timeInterval),
		//tv(),
		camera_r(30.0),
		camera_theta(270.0*PI/180.0),
		camera_phi(60.0*PI/180.0),
		//paths(),
		now(1460635200),//1460631600),//1456153155),
		timeMin(now),
		timeMax(now+60*60*24*3),//1453300000),
		mapTransform(),
		shaderProgramTexture(),
		shaderProgramPositionColor(),
		shaderProgramPaths(),
		map("ENRC1_20160204",shaderProgramTexture),
		map2("ENRC2_20160204",shaderProgramTexture),
		sector(shaderProgramPositionColor),
		path(shaderProgramPaths)
	{
		camera_target[0]=0.0;
		camera_target[1]=0.0;
		camera_target[2]=0.0;
	}
	
	~ATFViewerMainGLSL()
	{
		//Window を閉じる
		glfwDestroyWindow(window);
		//GLFW を破棄する
		glfwTerminate();
	}
	
	//シーンの初期化
	void initScene(void);
	//void initPathPoint(DBAccessor& dba);
	//void drawPath(PathPoint& p);
	//PathPoint getNowPoint(PathPoint& from, PathPoint& to, double time);
	//シングルトンとするためコピーコンストラクタ、代入演算子は定義しない
	ATFViewerMainGLSL(const ATFViewerMainGLSL& a);
	ATFViewerMainGLSL& operator=(const ATFViewerMainGLSL& a);
	//画面更新用の関数
	static void _display(void)
	{
		ATFViewerMainGLSL::getInstance().display();
	}
	//画面のリサイズ時に実行される関数
	static void _resize(int w, int h)
	{
		ATFViewerMainGLSL::getInstance().resize(w,h);
	}
	//キーボード入力時に実行される関数
	static void _keyboard(unsigned char key, int x, int y)
	{
		ATFViewerMainGLSL::getInstance().keyboard(key,x,y);
	}
	//ジョイスティックイベントの検出のための関数
	static void _joystickTimer(int value)
	{
		ATFViewerMainGLSL::getInstance().joystickTimer(value);
	}
	static void _idle(void)
	{
		ATFViewerMainGLSL::getInstance().idle();
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
		//glutPostRedisplay();
	}
	void initCallbacks()
	{
		//画面更新関数の設定
		//glutDisplayFunc(ATFViewerMainGLSL::_display);
		//画面リサイズ時に実行される関数の設定
		//glutReshapeFunc(ATFViewerMainGLSL::_resize);
		//キーボード入力時に実行される関数の設定
		//glutKeyboardFunc(ATFViewerMainGLSL::_keyboard);
		//ジョイスティックイベントの検出のための関数
		//glutTimerFunc(pollingInterval, ATFViewerMainGLSL::_joystickTimer, joystickTimerId);
		//アイドル時に実行される関数の設定
		//glutIdleFunc(ATFViewerMainGLSL::_idle);
		
	}
	void setMatrix(void);
public:
	//インスタンスの取得
	inline static ATFViewerMainGLSL& getInstance()
	{
		static ATFViewerMainGLSL atfvm;
		return atfvm;
	}
	void init(void)
	{
		std::cout << "ATFViewerMainGLSL::init()" << std::endl;
		
		//int argc=0;
		//char* argv[]={};
		
		/*
		//GLUTの初期化
		glutInit(&argc,argv);
		std::cout << "glutInit(&argc,argv) end." << std::endl;
		//ディスプレイモードの設定
		glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
		
		//メインループ終了後に処理を戻す
		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
		
		//ウィンドウの生成
		glutCreateWindow("ATFViewer");
		
		//GLEWの初期化
		GLenum err;
		err = glewInit();
		if(err != GLEW_OK)
		{
			std::cout << "error at glewInit()." << std::endl;
		}
		*/
		
		if(!glfwInit())
		{
			std::cout << "GLFW initialization failed." << std::endl;
		}
		//対応させるOpenGLのバージョンを指定する
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		
		//Window と、それに関連する OpenGL context を作成する
		window = glfwCreateWindow(640, 480, "glfwtest", NULL, NULL);
		if(!window)
		{
			std::cout << "Window initialization failed." << std::endl;
		}
		
		//デプスバッファを使用する
		glEnable(GL_DEPTH_TEST);
		
		//コールバック関数の設定
		this->initCallbacks();
		
		//シーンの初期化
		this->initScene();
	}
	void execMainLoop(void)
	{
		std::cout << "glutMainLoop() start." << std::endl;
		//glutMainLoop();
		//イベントループ
		while(!glfwWindowShouldClose(window))
		{
			//イベントの発生をポーリングする
			glfwPollEvents();
		}
		std::cout << "glutMainLoop() finished." << std::endl;
	}
};









