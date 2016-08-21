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

#ifndef Joystick_H_
#define Joystick_H_

#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

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



class Joystick
{
private:
	//ジョイスティックのデバイスのパス
	const std::string Device;
	//ファイルディスクリプタ
	int File;
	//軸の個数
	int AxisCount;
	//ボタンの個数
	int ButtonCount;
	//軸の状態を保持するvector
	std::vector<int> Axis;
	//ボタンの状態を保持するvector
	std::vector<bool> Button;
public:
	Joystick():
		Device("/dev/input/js0"),
		File(-1),
		AxisCount(0),
		ButtonCount(0),
		Axis(),
		Button()
	{
		if((File = open(Device.c_str(), O_RDONLY)) == -1)
		{
			AxisCount = 0;
			ButtonCount = 0;
		}
		else
		{
			//軸の個数を取得
			ioctl(File,    JSIOCGAXES,   &AxisCount);
			//軸の個数を設定
			Axis.resize(AxisCount,0);
			//ボタンの個数を取得
			ioctl(File, JSIOCGBUTTONS, &ButtonCount);
			//ボタンの個数を設定
			Button.resize(ButtonCount, 0);
			//ノンブロックモードに設定
			fcntl(File, F_SETFL, O_NONBLOCK );
		}
	}
	//ジョイスティック自体が有効かを返す
	//ジョイスティックが読み込めなかった場合はfalseを返す
	bool isEnable()
	{
		return (File != -1);
	}
	//イベントの読み取り
	//ノンブロックモードなのでイベントが無ければ更新されない
	//（イベントが起きるまで待つことは無い）
	void readJoystickEvent()
	{
		js_event event;
		
		//イベントの読み取り
		int ret = read(File, &event, sizeof(js_event));
		
		//読み取りできなかった場合は終了
		if(ret != sizeof(js_event))
		{
			return;
		}
		
		//発生したイベントの状態を取得する
		switch(event.type & (~JS_EVENT_INIT))
		{
		case JS_EVENT_AXIS:
			//軸の状態の更新
			Axis[event.number] = event.value;
			break;
		case JS_EVENT_BUTTON:
			//ボタンの状態の更新
			Button[event.number] = (event.value!=0);
			break;
		}
	}
	//軸の状態の取得
	int getAxisState(int i)
	{
		if(i < 0 || i >= AxisCount)
		{
			return 0;
		}
		return Axis[i];
	}
	//ボタンの状態の取得
	//true:押されている
	//false:押されていない
	bool getButtonState(int i)
	{
		if(i < 0 || i >= ButtonCount)
		{
			return false;
		}
		return Button[i];
	}
};

#endif
