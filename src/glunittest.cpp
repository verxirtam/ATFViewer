/*
 * =====================================================================================
 *
 *       Filename:  glunittest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月13日 18時38分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "TextureShaderProgram.h"
#include "MapVAO.h"

TextureShaderProgram* tsp;
MapVAO* map;


glm::mat4 projection;

void idle(void)
{
	glutPostRedisplay();
}

void resize(int w, int h)
{
	//ウィンドウのどの範囲をビューポートにするかを設定する
	//下記はウィンドウの全範囲をビューポートに設定している
	glViewport(0, 0, w, h);
	
	//プロジェクション行列の設定
	projection = glm::perspective( 3.141592f * 30.0f / 180.0f, static_cast<float>(w) / static_cast<float>(h), 1.0f, 1000.f);
}

void setMatrix(void)
{
	//回転する角度
	static int degree = 0;
	//ビュー変換行列の設定
	glm::mat4 view = glm::lookAt(
			glm::vec3(139.0f, 55.0f, 30.0f),//カメラの位置
			glm::vec3(139.0f, 35.0f,  0.0f),//視点の位置
			glm::vec3(  0.0f,  1.0f,  0.0f) //カメラの上方向の向き
			);
	//モデル変換行列の設定
	//y軸周りに回転させる行列
	//float d = 3.141592f * static_cast<float>(degree) / 180.0f;
	//glm::mat4 model = glm::rotate(glm::mat4(1.0f), d, glm::vec3(0.0f, 1.0f, 0.0f));
	
	//モデル・ビュー・プロジェクション行列を設定
	glm::mat4 mvp = projection * view;// * model;
	
	//角度の更新
	degree += 1;
	degree = (degree >= 360) ? (degree - 360) : degree;
	
	
	
	tsp->setMVPMatrix(mvp);
}
void display(void)
{
	//フレームバッファ、深度バッファをクリアする
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	setMatrix();
	
	map->display();
	
	//描画対象のバッファを入れ替える
	glutSwapBuffers();
}

void initCallbacks(void)
{
	//画面更新関数の設定
	glutDisplayFunc(display);
	//アイドル時に実行される関数の設定
	glutIdleFunc(idle);
	//画面リサイズ時に実行される関数の設定
	glutReshapeFunc(resize);
}
void initScene(void)
{
	tsp->init();
	
	DBAccessor dba("../../db/ATFViewer.db");
	map->init(dba);
}

int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	//ディスプレイモードの設定
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	//ウィンドウの生成
	glutCreateWindow("glunittest");
	//glewの初期化
	GLenum err;
	err = glewInit();
	if(err != GLEW_OK)
	{
		std::cout << "error at glewInit()." << std::endl;
		return 1;
	}
	//コールバック関数の設定
	initCallbacks();
	
	//cudaを使用するデバイスを指定する
	cudaSetDevice(0);
	
	tsp = new TextureShaderProgram();
	map = new MapVAO("ENRC2_20160204",*tsp);
	
	
	//デプスバッファを有効にする
	glEnable(GL_DEPTH_TEST);
	
	//シーンの初期化
	initScene();
	//メインループ
	glutMainLoop();
	
	delete tsp;
	
	return 0;
}
