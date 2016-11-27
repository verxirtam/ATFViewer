/*
 * =====================================================================================
 *
 *       Filename:  SceanNothing.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年11月26日 11時52分48秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include "Scean.h"

class SceanNothing : public Scean
{
public:
	//デストラクタ
	//継承される前提なので仮想関数として定義する
	virtual ~SceanNothing()
	{
	}
	//シーンの初期化を行う
	virtual void init()
	{
	
	}
	//画面更新用の関数
	virtual void display(GLFWwindow* window)
	{
	
	}
	//画面のリサイズ時に実行される関数
	virtual void resize(GLFWwindow* window, int w, int h)
	{
	
	}
	//キーボード入力時に実行される関数
	virtual void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
	
	}
	//ジョイスティックイベントの処理
	virtual void joystickInput(Joystick& joystick)
	{
	
	}
	
};


