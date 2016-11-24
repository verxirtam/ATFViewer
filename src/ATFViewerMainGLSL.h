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
#include <memory>

#include "OpenGLHeaders.h"
#include "Joystick.h"

#include "SceanLatLongFlow.h"

class ATFViewerMainGLSL
{
private:
	GLFWwindow* window;
	int windowWidth;
	int windowHeight;
	Joystick joystick;
	std::unique_ptr<Scean> scean;
	//初期化
	ATFViewerMainGLSL():
		window(nullptr),
		windowWidth(100),
		windowHeight(100),
		joystick(),
		scean(nullptr)
	{
	}
	
	~ATFViewerMainGLSL()
	{
		//Window を閉じる
		glfwDestroyWindow(window);
		//GLFW を破棄する
		glfwTerminate();
	}
	
	//シングルトンとするためコピーコンストラクタ、代入演算子、
	//ムーブコンストラクタ、ムーブ代入演算子はdeleteする
	ATFViewerMainGLSL(const ATFViewerMainGLSL& a) = delete;
	ATFViewerMainGLSL& operator=(const ATFViewerMainGLSL& a) = delete;
	ATFViewerMainGLSL(ATFViewerMainGLSL&&) = delete;
	ATFViewerMainGLSL& operator=(ATFViewerMainGLSL&&) = delete;
	//画面のリサイズ時に実行される関数
	static void _resize(GLFWwindow* window, int w, int h)
	{
		ATFViewerMainGLSL::getInstance().resize(window, w, h);
	}
	//キーボード入力時に実行される関数
	static void _keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		ATFViewerMainGLSL::getInstance().keyboard(window, key, scancode, action, mods);
	}
	//画面更新用の関数
	void display(GLFWwindow* window)
	{
		scean->display(window);
	}
	//画面のリサイズ時に実行される関数
	void resize(GLFWwindow* window, int w, int h)
	{
		scean->resize(window, w, h);
	}
	//キーボード入力時に実行される関数
	void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		scean->keyboard(window, key, scancode, action, mods);
	}
	//ジョイスティックイベントの処理
	void joystickInput()
	{
		scean->joystickInput(joystick);
	}
	void initCallbacks()
	{
		//画面リサイズ時に実行される関数の設定
		glfwSetWindowSizeCallback(window, ATFViewerMainGLSL::_resize);
		//キーボード入力時に実行される関数の設定
		glfwSetKeyCallback(window, ATFViewerMainGLSL::_keyboard);
	}
	//GLFWの初期化
	void initGLFW()
	{
		//GLFWの初期化
		if(!glfwInit())
		{
			std::cout << "GLFW initialization failed." << std::endl;
		}
		//対応させるOpenGLのバージョンを指定する
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}
	//描画対象となるWindowを作成する
	void createWindow()
	{
		//Window と、それに関連する OpenGL context を作成する
		window = glfwCreateWindow(640, 480, "glfwtest", NULL, NULL);
		if(!window)
		{
			std::cout << "Window initialization failed." << std::endl;
		}
		
		//GLで描画する画面を指定する
		glfwMakeContextCurrent(window);
		
	}
	//GLEWの初期化
	//GLFWが初期化され、描画対象のWindowが生成された後に実施する必要がある
	void initGLEW()
	{
		//GLEWの初期化
		GLenum err;
		err = glewInit();
		if(err != GLEW_OK)
		{
			std::cout << "error at glewInit()." << std::endl;
		}
		
	}
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
		
		//GLFWの初期化
		initGLFW();
		
		//描画対象となるWindowを作成する
		createWindow();
		
		//GLEWの初期化
		initGLEW();
		
		//バッファを切り替える間隔を指定する
		glfwSwapInterval(1);
		
		//デプスバッファを使用する
		glEnable(GL_DEPTH_TEST);
		
		//コールバック関数の設定
		this->initCallbacks();
		
		//シーンの初期化
		scean.reset(new SceanLatLongFlow());
		scean->init();
	}
	void execMainLoop(void)
	{
		std::cout << "execMainLoop() start." << std::endl;
		//glutMainLoop();
		//イベントループ
		while(!glfwWindowShouldClose(window))
		{
			//オブジェクトの表示
			display(window);
			
			//ジョイスティックによる入力の処理
			joystickInput();
			
			//イベントの発生をポーリングする
			glfwPollEvents();
		}
		std::cout << "execMainLoop() finished." << std::endl;
	}
};









