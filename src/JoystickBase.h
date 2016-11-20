/*
 * =====================================================================================
 *
 *       Filename:  JoystickBase.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年11月19日 21時35分03秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once


template <typename JoystickManager>
class JoystickBase
{
private:
	JoystickManager joystickManager;
public:
	JoystickBase():
		joystickManager()
	{
	}
	//ジョイスティック自体が有効かを返す
	//ジョイスティックが読み込めなかった場合はfalseを返す
	bool isEnable()
	{
		return joystickManager.isEnable();
	}
	//イベントの読み取り
	//ノンブロックモードなのでイベントが無ければ更新されない
	//（イベントが起きるまで待つことは無い）
	void readJoystickEvent()
	{
		joystickManager.readJoystickEvent();
	}
	//軸の状態の取得
	float getAxisState(int i)
	{
		return joystickManager.getAxisState(i);
	}
	//ボタンの状態の取得
	//true:押されている
	//false:押されていない
	bool getButtonState(int i)
	{
		return joystickManager.getButtonState(i);
	}
};

