/*
 * =====================================================================================
 *
 *       Filename:  Scean.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年11月24日 21時42分18秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "OpenGLHeaders.h"
#include "Joystick.h"

class Scean
{
public:
	//デストラクタ
	//継承される前提なので仮想関数として定義する
	virtual ~Scean()
	{
	}
	//シーンの初期化を行う
	virtual void init() = 0;
	//画面更新用の関数
	virtual void display(GLFWwindow* window) = 0;
	//画面のリサイズ時に実行される関数
	virtual void resize(GLFWwindow* window, int w, int h) = 0;
	//キーボード入力時に実行される関数
	virtual void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
	//ジョイスティックイベントの処理
	virtual void joystickInput(Joystick& joystick) = 0;
	
};
