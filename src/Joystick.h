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

#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

class Joystick
{
private:
	//ジョイスティックのデバイスのパス
	const char* Device;
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
		if((File = open(Device, O_RDONLY)) == -1)
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
		read(File, &event, sizeof(js_event));
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
