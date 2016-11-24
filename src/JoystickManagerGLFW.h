/*
 * =====================================================================================
 *
 *       Filename:  JoystickManagerSystem.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年11月19日 21時31分13秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include "OpenGLHeaders.h"


class JoystickManagerGLFW
{
private:
	//軸の個数
	int axisCount;
	//軸の状態
	const float* axisState;
	//ボタンの個数
	int buttonCount;
	//ボタンの状態
	const unsigned char* buttonState;
public:
	JoystickManagerGLFW():
		axisCount(0),
		axisState(nullptr),
		buttonCount(0),
		buttonState(nullptr)
	{
		if(this->isEnable())
		{
			readJoystickEvent();
		}
	}
	//ジョイスティック自体が有効かを返す
	//ジョイスティックが読み込めなかった場合はfalseを返す
	bool isEnable() const
	{
		return (glfwJoystickPresent(GLFW_JOYSTICK_1) == GLFW_TRUE);
	}
	//イベントの読み取り
	void readJoystickEvent()
	{
		axisState = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axisCount);
		buttonState = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
	}
	//軸の状態の取得
	float getAxisState(int i) const
	{
		if(i < 0 || i >= axisCount)
		{
			return 0;
		}
		return axisState[i];
	}
	//ボタンの状態の取得
	//true:押されている
	//false:押されていない
	bool getButtonState(int i) const
	{
		if(i < 0 || i >= buttonCount)
		{
			return false;
		}
		return (buttonState[i] == GLFW_PRESS);
	}
};


