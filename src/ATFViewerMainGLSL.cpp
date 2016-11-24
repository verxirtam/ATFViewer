/*
 * =====================================================================================
 *
 *       Filename:  ATFViewerMainGLSL.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年02月02日 00時53分02秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
/*
#include "ATFViewerMainGLSL.h"

using namespace std;


void ATFViewerMainGLSL::initScene(void)
{
	cout<<"initScene() start"<<endl;
	
	shaderProgramTexture.init();
	shaderProgramPositionColor.init();
	shaderProgramPaths.init();
	
	DBAccessor dba("../../db/ATFViewer.db");
	map.init(dba);
	map2.init(dba);
	map3.init(dba);
	sector.init(dba);
	path.initPathPoint(this->timeMin, this->timeMax);
	
	cout<<"initScene() end"<<endl;
}

void ATFViewerMainGLSL::setMatrix(void)
{
	glm::mat4 projection = glm::perspective
		(
			3.141592f * 30.0f / 180.0f,
			static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
			1.0f,
			1000.f
		);
	
	glm::vec3 camera_state
		(
			camera_target[0] + camera_r * cos(camera_phi) * cos(camera_theta),
			camera_target[1] + camera_r * cos(camera_phi) * sin(camera_theta),
			camera_target[2] + camera_r * sin(camera_phi)
		);
	glm::mat4 view = glm::lookAt
		(
			camera_state,//カメラの位置
			glm::vec3(camera_target[0], camera_target[1],  camera_target[2]),//視点の位置
			glm::vec3(  0.0f,  0.0f,  1.0f) //カメラの上方向の向き
		);
	glm::mat4 model(1.0);
	mapTransform.setTransform(model);
	
	glm::mat4 mvp(projection * view * model);
	
	shaderProgramTexture.setMVPMatrix(mvp);
	shaderProgramPositionColor.setMVPMatrix(mvp);
	shaderProgramPaths.setMVPMatrix(mvp);
}

void ATFViewerMainGLSL::display(GLFWwindow* window)
{
	//時刻の更新
	now += currentTimeInterval;
	if (now > timeMax)
	{
		now = timeMin;
		path.resetTime();
	}
	
	//フレームバッファ、深度バッファをクリアする
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	setMatrix();
	
	map.display();
	map2.display();
	map3.display();
	sector.display();
	path.display(now);
	
	//描画対象のバッファを入れ替える
	glfwSwapBuffers(window);
}


void ATFViewerMainGLSL::resize(GLFWwindow* window, int w, int h)
{
	//ウィンドウのどの範囲をビューポートにするかを設定する
	//下記はウィンドウの全範囲をビューポートに設定している
	glViewport(0, 0, w, h);

	windowWidth = w;
	windowHeight = h;
}

void ATFViewerMainGLSL::keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//GLFW_PRESS か GLFW_REPEATのイベント以外は無視する
	if((action != GLFW_PRESS) && (action != GLFW_REPEAT))
	{
		return;
	}
	//キーが押された時の処理
	switch (key)
	{
	case GLFW_KEY_F://'f':
		//center_offset_long += 1.0 / scale;
		mapTransform.setCenterOffsetLong(mapTransform.getCenterOffsetLong() + 1.0 / mapTransform.getScale());
		display(window);
		break;
	case GLFW_KEY_S://'s':
		//center_offset_long -= 1.0 / scale;
		mapTransform.setCenterOffsetLong(mapTransform.getCenterOffsetLong() - 1.0 / mapTransform.getScale());
		display(window);
		break;
	case GLFW_KEY_E://'e':
		//center_offset_lat += 1.0 / scale;
		mapTransform.setCenterOffsetLat(mapTransform.getCenterOffsetLat() + 1.0 / mapTransform.getScale());
		display(window);
		break;
	case GLFW_KEY_D://'d':
		//center_offset_lat -= 1.0 / scale;
		mapTransform.setCenterOffsetLat(mapTransform.getCenterOffsetLat() - 1.0 / mapTransform.getScale());
		display(window);
		break;
	case GLFW_KEY_L://'l':
		camera_theta+=2.5*PI/180.0;
		camera_theta = (camera_theta > 2.0 * PI) ? camera_theta-2.0*PI : camera_theta;
		display(window);
		break;
	case GLFW_KEY_J://'j':
		camera_theta-=2.5*PI/180.0;
		camera_theta = (camera_theta < 0.0) ? camera_theta+2.0*PI : camera_theta;
		display(window);
		break;
	case GLFW_KEY_I://'i':
		camera_phi+=5.0*PI/180.0;
		camera_phi = (camera_phi > 90.0*PI/180.0) ? 90.0*PI/180.0 : camera_phi;
		display(window);
		break;
	case GLFW_KEY_K://'k':
		camera_phi-=5.0*PI/180.0;
		camera_phi = (camera_phi < 0.0) ? 0.0*PI/180.0 : camera_phi;
		display(window);
		break;
	case GLFW_KEY_B://'b':
		//scale*=0.875;
		mapTransform.setScale(mapTransform.getScale()*0.875);
		display(window);
		break;
	case GLFW_KEY_SPACE://' ':
		//scale/=0.875;
		mapTransform.setScale(mapTransform.getScale()/0.875);
		display(window);
		break;
	case GLFW_KEY_T://'t':
		std::cout<<::asctime(::localtime(&now))<<std::endl;
		static bool animation_enable = true;
		if(animation_enable)
		{
			//glutIdleFunc(ATFViewerMainGLSL::_idle);
			animation_enable = false;
			currentTimeInterval = 0;
		}
		else
		{
			//glutIdleFunc(NULL);
			animation_enable = true;
			currentTimeInterval = timeInterval;
		}
		break;
	case GLFW_KEY_Y://'y':
		//表示するセクタを切り替える
		//sectors.switchDisplaySector();
		break;
	}
}


//ジョイスティックイベントの検出のための関数
void ATFViewerMainGLSL::joystickInput()
{
	//ジョイスティックの状況を取得する
	if(joystick.isEnable())
	{
		joystick.readJoystickEvent();
	}
	else
	{
		return;
	}
	
	//ポーリング間隔に応じた倍率
	double pr = 10.0 / 100.0;
	
	
	//各軸の傾きに応じた割合
	const float baffer = 0.125f;
	
	//ジョイスティックの操作が行われたかを表す
	bool occurred = false;
	
	//X軸:左スティック左右
	float xd = 0.0f;
	float x = joystick.getAxisState(Axis_LeftStick_LeftRight);
	//Y軸:左スティック上下
	float yd = 0.0f;
	float y = joystick.getAxisState(Axis_LeftStick_UpDown);
	if((x >= baffer) || (x < -baffer) || (y >= baffer) || (y < -baffer))
	{
		xd = x * pr;
		yd = y * pr;
		occurred = true;
	}
	
	//U軸:右スティック左右
	float ud = 0.0f;
	float u = joystick.getAxisState(Axis_RightStick_LeftRight);
	//V軸:右スティック上下
	float vd = 0.0f;
	float v = joystick.getAxisState(Axis_RightStick_UpDown);
	if((u >= baffer) || (u < -baffer) || (v >= baffer) || (v < -baffer))
	{
		ud = u * pr;
		vd = v * pr;
		occurred = true;
	}
	
	
	//マップの拡大縮小率
	float rd = 1.0f;
	
	//左トリガー
	float p = joystick.getAxisState(Axis_LeftTrigger_OffOn);
	
	//右トリガー
	float q = joystick.getAxisState(Axis_RightTrigger_OffOn);
	if((p >= - 1.0f + baffer) || (q >= - 1.0f + baffer))
	{
		float pd = (p + 1.0f) / 2.0f * pr;
		float qd = (q + 1.0f) / 2.0f * pr;
		rd = 1.0f - 0.125f * (pd - qd);
		
		occurred = true;
	}
	
	
	//ジョイスティック操作が行われた時の処理
	if(occurred)
	{
		//視点の位置の移動
		//x:視点の位置(経度方向)の移動
		mapTransform.setCenterOffsetLong(mapTransform.getCenterOffsetLong() + xd / mapTransform.getScale());
		
		//y:視点の位置(緯度方向)の移動
		mapTransform.setCenterOffsetLat(mapTransform.getCenterOffsetLat() - yd / mapTransform.getScale());
		
		//カメラの移動
		//x:カメラを左右に移動
		camera_theta += 5.0 * PI / 180.0 * ud;
		camera_theta = (camera_theta > 2.0 * PI) ? camera_theta-2.0*PI : camera_theta;
		camera_theta = (camera_theta < 0.0) ? camera_theta+2.0*PI : camera_theta;
		
		//y:カメラを上下に移動
		camera_phi -= 5.0 * PI / 180.0 * vd;
		camera_phi = (camera_phi > 90.0*PI/180.0) ? 90.0*PI/180.0 : camera_phi;
		camera_phi = (camera_phi < 0.0) ? 0.0*PI/180.0 : camera_phi;
		
		//マップの拡大縮小
		mapTransform.setScale(mapTransform.getScale() * rd);
	}
}
*/
