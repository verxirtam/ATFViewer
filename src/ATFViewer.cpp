/*
 * =====================================================================================
 *
 *       Filename:  ATFViewer.cpp
 *
 *    Description:  OpenGLで航空機の位置を表示する
 *
 *        Version:  1.0
 *        Created:  2016年01月24日 01時33分50秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  verxirtam 
 *   Organization:  
 *
 * =====================================================================================
 */

/*
 * 長さの単位はcm
 * 机の上に地図を広げたイメージ
 * 高度方向は実寸だと見分けがつかないので拡大する
 * */

#include <iostream>
#include <cstdio>
#include <cmath>
#include <GL/glut.h>

#include "DBAccessor.h"

#define TEXWIDTH  (600)
#define TEXHEIGHT (600)

using namespace std;

#define PI (3.14159265358979)

GLdouble camera_r=30.0;
GLdouble camera_theta=300.0*PI/180.0;
GLdouble camera_phi=60.0*PI/180.0;
GLdouble camera_target[]={0.0,0.0,0.0};


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	//モデルビュー変換行列の初期化
	glLoadIdentity();
	
	//ビュー変換行列の設定
	//カメラの位置、向きの設定
	double cx=camera_target[0] + camera_r * cos(camera_phi) * cos(camera_theta);
	double cy=camera_target[1] + camera_r * cos(camera_phi) * sin(camera_theta);
	double cz=camera_target[2] + camera_r * sin(camera_phi);
	gluLookAt(
			cx,cy,cz,//カメラの位置
			camera_target[0],camera_target[1],camera_target[2],//視点の位置
			0.0,   0.0,  1.0 //カメラから見た垂直方向
			);
	
	//光源の設定
	const static GLfloat light0pos[] = { 0.0, 0.0, 100.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	
	//モデリング変換行列の設定
	//羽田がワールド座標系の原点に来るように平行移動する
	double haneda_x = 139.0 + ( 46.0/60.0 + 87.0/3600.0 );
	double haneda_y =  35.0 + ( 33.0/60.0 + 20.0/3600.0 );
	glTranslated( - haneda_x, - haneda_y, 0.0);
	//モデル座標は経緯度-feetのサイズになっているので
	//地図っぽくスケーリングする
	//経緯度はそのままで行けそうなので、feetのみ変更
	//40000feetが4cmになるように1/10000に縮小する
	glScaled(1.0,1.0,0.0001);
	
	//世界地図を描く
	//材質の設定
	static const GLfloat color[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	glNormal3d(0.0,0.0,1.0);
	glTexCoord2d(0.0,0.0);glVertex3d( 90.0,90.0,0.0);//-180.0, 90.0,0.0);
	glTexCoord2d(0.0,1.0);glVertex3d( 90.0, 0.0,0.0);//-180.0,-90.0,0.0);
	glTexCoord2d(1.0,0.0);glVertex3d(180.0,90.0,0.0);// 180.0, 90.0,0.0);
	glNormal3d(0.0,0.0,1.0);
	glTexCoord2d(0.0,1.0);glVertex3d( 90.0, 0.0,0.0);//-180.0,-90.0,0.0);
	glTexCoord2d(1.0,1.0);glVertex3d(180.0, 0.0,0.0);// 180.0,-90.0,0.0);
	glTexCoord2d(1.0,0.0);glVertex3d(180.0,90.0,0.0);// 180.0, 90.0,0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//世界地図の枠を描く
	glBegin(GL_LINE_LOOP);
	glVertex3d( 90.0,90.0,0.0);//-180.0, 90.0,0.0);
	glVertex3d( 90.0, 0.0,0.0);//-180.0,-90.0,0.0);
	glVertex3d(180.0, 0.0,0.0);// 180.0,-90.0,0.0);
	glVertex3d(180.0,90.0,0.0);// 180.0, 90.0,0.0);
	glEnd();
	

	//羽田の上空にシンボルを描く
	glColor3d(1.0,1.0,1.0);
	for (int i = 0; i < 10; i++)
	{
		double id = (double)i;
		double d = 0.1;
		glBegin(GL_LINE_LOOP);
		glVertex3d(haneda_x-d, haneda_y-d, 5000.0*id);
		glVertex3d(haneda_x+d, haneda_y-d, 5000.0*id);
		glVertex3d(haneda_x+d, haneda_y+d, 5000.0*id);
		glVertex3d(haneda_x-d, haneda_y+d, 5000.0*id);
		glEnd();
	}
	glBegin(GL_LINE_LOOP);
	glVertex3d(haneda_x-10.0, haneda_y-10.0,0.0);
	glVertex3d(haneda_x+10.0, haneda_y-10.0,0.0);
	glVertex3d(haneda_x+10.0, haneda_y+10.0,0.0);
	glVertex3d(haneda_x-10.0, haneda_y+10.0,0.0);
	glEnd();
	

	//航空機の軌道っぽいものを描く
	static const GLdouble trj[][3]
		={
			{139.0, 35.0,     0.0},
			{139.0, 36.0,  1000.0},
			{139.0, 37.0,  5000.0},
			{140.0, 38.0, 10000.0},
			{141.0, 36.0, 30000.0},
			{142.0, 35.0, 30000.0},
			{143.0, 34.0, 15000.0},
			{144.0, 33.0,  1000.0},
			{144.0, 32.0,   500.0},
			{144.0, 31.0,    0.0},
		};
	glBegin(GL_LINES);
	for (int i = 0; i < 10-1; i++)
	{
		glVertex3d(trj[i  ][0],trj[i  ][1],trj[i  ][2]);
		glVertex3d(trj[i+1][0],trj[i+1][1],trj[i+1][2]);
		glVertex3d(trj[i  ][0],trj[i  ][1],trj[i  ][2]);
		glVertex3d(trj[i  ][0],trj[i  ][1],        0.0);
	}
	glVertex3d(trj[9][0],trj[9][1],trj[9][2]);
	glVertex3d(trj[9][0],trj[9][1],      0.0);
	glEnd();
	
	glFlush();

}

void resize(int w, int h)
{

	//ウィンドウのどの範囲をビューポートにするかを設定する
	//下記はウィンドウの全範囲をビューポートに設定している
	glViewport(0, 0, w, h);

	//変換行列の初期化
	//透視変換行列の設定
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, ((double)w)/(double)h, 1.0, 1000.0);

	//モデルビュー変換行列の設定
	//モデリング変換（モデル- -> ワールド）、
	//ビューイング変換(ワールド -> カメラ)
	//を行うための設定
	glMatrixMode(GL_MODELVIEW);

}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'f':
		camera_target[0]+=1.0;
		display();
		break;
	case 's':
		camera_target[0]-=1.0;
		display();
		break;
	case 'e':
		camera_target[1]+=1.0;
		display();
		break;
	case 'd':
		camera_target[1]-=1.0;
		display();
		break;
	case 'l':
		camera_theta+=10.0*PI/180.0;
		camera_theta = (camera_theta > 2.0 * PI) ? camera_theta-2.0*PI : camera_theta;
		display();
		break;
	case 'j':
		camera_theta-=10.0*PI/180.0;
		camera_theta = (camera_theta < 0.0) ? camera_theta+2.0*PI : camera_theta;
		display();
		break;
	case 'i':
		camera_phi+=5.0*PI/180.0;
		camera_phi = (camera_phi >80.0*PI/180.0) ? 80.0*PI/180.0 : camera_phi;
		display();
		break;
	case 'k':
		camera_phi-=5.0*PI/180.0;
		camera_phi = (camera_phi < 0.0) ? 0.0*PI/180.0 : camera_phi;
		display();
		break;
	case 'b':
		camera_r/=0.875;
		camera_r = (camera_r > 10000.0) ? 10000.0 : camera_r;
		display();
		break;
	case ' ':
		camera_r*=0.875;
		camera_r = (camera_r < 1.0) ? 1.0 : camera_r;
		display();
		break;
	}
}

void init(void)
{
 
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//テクスチャの読み込み
	//テクスチャ読み込み用の配列
	GLubyte texture[TEXWIDTH][TEXHEIGHT][3];
	//テクスチャファイルのファイルポインタ
	std::FILE* fp;
	//テクスチャファイル名
	const char texture1[]="/home/daisuke/programs/ATFViewer/res/el_v2_0600_090-180_00-90.raw";
	//テクスチャファイルの読み込み
	if ((fp = std::fopen(texture1, "rb")) != NULL)
	{
		std::fread(texture, sizeof(texture),1,fp);
		std::fclose(fp);
	}
	else
	{
		printf("error.\n");
		std::perror(texture1);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//テクスチャの割り当て
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, TEXWIDTH, TEXHEIGHT, GL_RGB , GL_UNSIGNED_BYTE, texture);
	//テクスチャを拡大・縮小するときの保管方法を設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

int main(int argc, char *argv[])
{
	/* 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	init();
	glutMainLoop();
	*/
	cout<<"dba() before"<<endl;
	DBAccessor dba(std::string("../../db/ATFViewer.db"));
	cout<<"dba() after, setQuery() before"<<endl;
	dba.setQuery(std::string("select longitude,latitude,altitude,time from TrackData where id='895024a' order by time;"));
	cout<<"setQuery() after, step_select() before"<<endl;
	while(SQLITE_ROW == dba.step_select())
	{
		cout<<"("<<dba.getColumnDouble(0)<<",";
		cout<<dba.getColumnDouble(1)<<",";
		cout<<dba.getColumnInt(2)<<",";
		cout<<dba.getColumnInt(3)<<")"<<endl;
	}
	
	return 0;
}

