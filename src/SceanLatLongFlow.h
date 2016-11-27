/*
 * =====================================================================================
 *
 *       Filename:  SceanLatLongFlow.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年11月24日 22時18分32秒
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

#include "Scean.h"

#include "OpenGLHeaders.h"

#include "DBAccessor.h"
#include "MapTransform.h"
#include "Joystick.h"
#include "MapVAO.h"
#include "SectorsVAO.h"
#include "PathsVAO.h"

class SceanLatLongFlow : public Scean
{
private:
	static const float pi;
	int windowWidth;
	int windowHeight;
	const int timeInterval;
	int currentTimeInterval;
	GLdouble camera_r;
	GLdouble camera_theta;
	GLdouble camera_phi;
	GLdouble camera_target[3];
	time_t now;
	const time_t timeMin;
	const time_t timeMax;
	MapTransform mapTransform;
	ShaderProgramTexture shaderProgramTexture;
	ShaderProgramPositionColor shaderProgramPositionColor;
	ShaderProgramPaths shaderProgramPaths;
	MapVAO map;
	MapVAO map2;
	MapVAO map3;
	SectorsVAO sector;
	PathsVAO path;
	void setMatrix(void);
public:

	SceanLatLongFlow():
		windowWidth(100),
		windowHeight(100),
		timeInterval(20),//5),
		currentTimeInterval(timeInterval),
		camera_r(30.0),
		camera_theta(270.0 * pi / 180.0),
		camera_phi(60.0 * pi / 180.0),
		now(1460635200),//1460631600),//1456153155),
		timeMin(now),
		timeMax(now+60*60*24*3),//1453300000),
		mapTransform(),
		shaderProgramTexture(),
		shaderProgramPositionColor(),
		shaderProgramPaths(),
		map("ENRC1_20160204",shaderProgramTexture),
		map2("ENRC2_20160204",shaderProgramTexture),
		map3("ENRC3_20160204",shaderProgramTexture),
		sector(shaderProgramPositionColor),
		path(shaderProgramPaths)
	{
	}

	//デストラクタ
	//継承される前提なので仮想関数として定義する
	virtual ~SceanLatLongFlow()
	{
	}
	//シーンの初期化を行う
	virtual void init() override;
	//画面更新用の関数
	virtual void display(GLFWwindow* window) override;
	//画面のリサイズ時に実行される関数
	virtual void resize(GLFWwindow* window, int w, int h) override;
	//キーボード入力時に実行される関数
	virtual void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) override;
	//ジョイスティックイベントの処理
	virtual void joystickInput(Joystick& joystick) override;
};


