/*
 * =====================================================================================
 *
 *       Filename:  Joystick.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年04月04日 23時16分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include "JoystickBase.h"
#include "JoystickManagerSystem.h"
#include "JoystickManagerGLFW.h"


enum KeyCode_F310
{
	Axis_LeftStick_LeftRight = 0,
	Axis_LeftStick_UpDown = 1,
	Axis_LeftTrigger_OffOn = 2,
	Axis_RightStick_LeftRight = 3,
	Axis_RightStick_UpDown = 4,
	Axis_RightTrigger_OffOn = 5,
	Button_A = 0,
	Button_B = 1,
	Button_X = 2,
	Button_Y = 3,
	Button_LB = 4,
	Button_RB = 5,
	Button_BACK = 6,
	Button_START = 7,
	Button_Logicool = 8,
	Button_LeftStick_Push = 9,
	Button_RightStick_Push = 10
};


using Joystick = JoystickBase<JoystickManagerSystem>;

