/*
 * =====================================================================================
 *
 *       Filename:  ATFViewerMain.h
 *
 *    Description:  メインのクラス
 *					OpenGLの設定を保持し、画面の制御を行う。
 *					シングルトンとして定義する。
 *					OpenGLの管理元を一意にするため。
 *        Version:  1.0
 *        Created:  2016年02月01日 22時25分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef ATFViewerMain_H
#define ATFViewerMain_H

#include <GL/glut.h>
#include <cstdio>

class ATFViewerMain
{
private:
	//初期化
	ATFViewerMain()
	{
		
	}
	//シーンの初期化
	void initScene(void);
	//シングルトンとするためコピーコンストラクタ、代入演算子は定義しない
	ATFViewerMain(const ATFViewerMain& a);
	ATFViewerMain& operator=(const ATFViewerMain& a);
	//画面更新用の関数
	static void _display(void)
	{
		ATFViewerMain::getInstance().display();
	}
	//画面のリサイズ時に実行される関数
	static void _resize(int w, int h)
	{
		ATFViewerMain::getInstance().resize();
	}
	//キーボード入力時に実行される関数
	static void _keyboard(unsigned char key, int x, int y)
	{
		ATFViewerMain::getInstance().keyboard(key,x,y);
	}
	//画面更新用の関数
	void display(void);
	//画面のリサイズ時に実行される関数
	void resize(void);
	//キーボード入力時に実行される関数
	void keyboard(unsigned char key, int x, int y);
public:
	//インスタンスの取得
	inline static ATFViewerMain& getInstance()
	{
		static ATFViewerMain atfvm;
		return atfvm;
	}
	void init(void)
	{
		int argc=0;
		char* argv[]={};
		//GLUTの初期化
		glutInit(&argc,argv);
		//ディスプレイモードの設定
		glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
		//ウィンドウの生成
		glutCreateWindow("ATFViewer");
		//画面更新関数の設定
		glutDisplayFunc(ATFViewerMain::_display);
		//画面リサイズ時に実行される関数の設定
		glutReshapeFunc(ATFViewerMain::_resize);
		//キーボード入力時に実行される関数の設定
		glutKeyboardFunc(ATFViewerMain::_keyboard);
		//シーンの初期化
		this->initScene();
	}
	void execMainLoop(void)
	{
		glutMainLoop();
	}
};








#endif

