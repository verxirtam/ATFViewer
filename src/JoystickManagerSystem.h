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


#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>


class JoystickManagerSystem
{
private:
	//ジョイスティックのデバイスのパス
	const std::string device;
	//ファイルディスクリプタ
	int file;
	//軸の個数
	int axisCount;
	//ボタンの個数
	int buttonCount;
	//軸の値の最大値
	const float axisValueMax = 32767.0;
	//軸の状態を保持するvector
	std::vector<int> axis;
	//ボタンの状態を保持するvector
	std::vector<bool> button;
public:
	JoystickManagerSystem():
		device("/dev/input/js0"),
		file(-1),
		axisCount(0),
		buttonCount(0),
		axis(),
		button()
	{
		if((file = open(device.c_str(), O_RDONLY)) == -1)
		{
			axisCount = 0;
			buttonCount = 0;
		}
		else
		{
			//軸の個数を取得
			ioctl(file,    JSIOCGAXES,   &axisCount);
			//軸の個数を設定
			axis.resize(axisCount,0);
			//ボタンの個数を取得
			ioctl(file, JSIOCGBUTTONS, &buttonCount);
			//ボタンの個数を設定
			button.resize(buttonCount, 0);
			//ノンブロックモードに設定
			fcntl(file, F_SETFL, O_NONBLOCK );
		}
	}
	//ジョイスティック自体が有効かを返す
	//ジョイスティックが読み込めなかった場合はfalseを返す
	bool isEnable()
	{
		return (file != -1);
	}
	//イベントの読み取り
	//ノンブロックモードなのでイベントが無ければ更新されない
	//（イベントが起きるまで待つことは無い）
	void readJoystickEvent()
	{
		js_event event;
		
		//イベントの読み取り
		int ret = read(file, &event, sizeof(js_event));
		
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
			axis[event.number] = event.value;
			break;
		case JS_EVENT_BUTTON:
			//ボタンの状態の更新
			button[event.number] = (event.value!=0);
			break;
		}
	}
	//軸の状態の取得
	float getAxisState(int i)
	{
		if(i < 0 || i >= axisCount)
		{
			return 0;
		}
		return static_cast<float>(axis[i]) / axisValueMax;
	}
	//ボタンの状態の取得
	//true:押されている
	//false:押されていない
	bool getButtonState(int i)
	{
		if(i < 0 || i >= buttonCount)
		{
			return false;
		}
		return button[i];
	}
};


