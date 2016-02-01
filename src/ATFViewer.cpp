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
/*
#include <iostream>
#include <cstdio>
#include <cmath>
#include <GL/glut.h>

#include <vector>

#include "DBAccessor.h"

#define TEXWIDTH  (600)
#define TEXHEIGHT (600)

using namespace std;

#define PI (3.14159265358979)

GLdouble camera_r=30.0;
GLdouble camera_theta=300.0*PI/180.0;
GLdouble camera_phi=60.0*PI/180.0;
GLdouble camera_target[]={0.0,0.0,0.0};

GLdouble center_offset_long = 0.0;
GLdouble center_offset_lat  = 0.0;
GLdouble scale=1.0;

time_t now;

struct PathPoint
{
	double longitude;
	double latitude;
	int altitude;
	long long time;
	PathPoint(double lo,double la, int a, long long t)
	{
		longitude=lo;
		latitude=la;
		altitude=a;
		time=t;
	}
};

vector<vector<PathPoint> > paths;

void drawPath(PathPoint& p)
{
	//航空機の高度に応じて色を設定する
	//帯の地面に接する箇所はアルファを0にする（完全に透明にする）
	double c=((double)p.altitude)/40000.0;
	double alfa = 1.0 - (((double)(now - p.time)) / 600.0);
	c=c*c;
	glColor4d(c, 0.5, 1.0 - c, alfa * (1.0 - c));
	glVertex3d(p.longitude, p.latitude, p.altitude);
	glColor4d(c,0.5,1.0-c,0.0);
	glVertex3d(p.longitude, p.latitude, 0.0);
}
PathPoint getNowPoint(PathPoint& from, PathPoint& to)
{
	PathPoint ret(0.0, 0.0, 0, 0);
	double ratio_from = 1.0 - ((double)(now - from.time))/((double)(to.time - from.time));
	double ratio_to = 1.0 - ratio_from;
	ret.longitude = ratio_from * from.longitude + ratio_to * to.longitude;
	ret.latitude = ratio_from * from.latitude + ratio_to * to.latitude;
	ret.altitude =(int)( ratio_from * ((double)from.altitude) + ratio_to * ((double)to.altitude));
	ret.time = now;
	return ret;
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
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
	//const static GLfloat light0pos[] = { 0.0, 0.0, 100.0, 1.0 };
	//glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	
	
	//クリッピング平面を設定
	double cp[][4]=
		{
			{-1.0, 0.0, 0.0, 20.0},
			{ 1.0, 0.0, 0.0, 20.0},
			{ 0.0,-1.0, 0.0, 20.0},
			{ 0.0, 1.0, 0.0, 20.0}
		};
	glClipPlane(GL_CLIP_PLANE0,cp[0]);
	glClipPlane(GL_CLIP_PLANE1,cp[1]);
	glClipPlane(GL_CLIP_PLANE2,cp[2]);
	glClipPlane(GL_CLIP_PLANE3,cp[3]);
	//クリッピング平面の設定
	glEnable(GL_CLIP_PLANE0);
	glEnable(GL_CLIP_PLANE1);
	glEnable(GL_CLIP_PLANE2);
	glEnable(GL_CLIP_PLANE3);
	
	//モデリング変換行列の設定
	//モデル座標は経緯度-feetのサイズになっているので
	//地図っぽくスケーリングする
	//経緯度はそのままで行けそうなので、feetのみ変更
	//40000feetが4cmになるように1/10000に縮小する
	glScaled(scale,scale,0.0001);
	//羽田+offsetがワールド座標系の原点に来るように平行移動する
	double haneda_x = 139.0 + ( 46.0/60.0 + 87.0/3600.0 );
	double haneda_y =  35.0 + ( 33.0/60.0 + 20.0/3600.0 );
	glTranslated( - haneda_x - center_offset_long, - haneda_y - center_offset_lat, 0.0);
	
	//世界地図を描く
	//材質の設定
	//static const GLfloat color[] = { 1.0, 1.0, 1.0, 1.0 };
	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glColor3d(1.0,1.0,1.0);
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
	for (int i = 0; i < 10; i++)
	{
		double id = (double)i;
		double d = 0.1;
		glBegin(GL_LINE_LOOP);
		glColor3d(1.0,1.0,0.0);
		glVertex3d(haneda_x-d, haneda_y-d, 5000.0*id);
		glVertex3d(haneda_x+d, haneda_y-d, 5000.0*id);
		glVertex3d(haneda_x+d, haneda_y+d, 5000.0*id);
		glVertex3d(haneda_x-d, haneda_y+d, 5000.0*id);
		glEnd();
	}
	glBegin(GL_LINE_LOOP);
	glColor3d(1.0,1.0,1.0);
	glVertex3d(haneda_x-10.0, haneda_y-10.0,0.0);
	glVertex3d(haneda_x+10.0, haneda_y-10.0,0.0);
	glVertex3d(haneda_x+10.0, haneda_y+10.0,0.0);
	glVertex3d(haneda_x-10.0, haneda_y+10.0,0.0);
	glEnd();
	

	//航空機の軌道っぽいものを描く
	//隠面消去を無効にする
	//アルファブレンドで奥の透明オブジェクトが描画されないことがあるため
	//また、軌道の手前に描かれるオブジェクトがないため最後に描画すれば前後関係は崩れない
	glDisable(GL_DEPTH_TEST);
	//アルファブレンド有効化
	glEnable(GL_BLEND);
	//アルファブレンドの方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//航空機毎に軌道を描画する
	for (unsigned int n = 0; n < paths.size(); n++)
	{
		//未来の軌道は描かない
		if (paths[n][0].time > now)
		{
			continue;
		}
		glBegin(GL_TRIANGLE_STRIP);
		//航空機の軌道に垂線をおろした帯状の図形を描画する
		int path_size=paths[n].size();
		for (int i = 0; i < path_size-1; i++)
		{
			if(paths[n][i].time > now)
			{
				PathPoint now_point = getNowPoint(paths[n][i-1],paths[n][i]);
				drawPath(now_point);
				break;
			}
			else
			{
				if(paths[n][i].time >= (now - 600) )
				{
					drawPath(paths[n][i]);
				}
			}
		}
		glEnd();
	}
	//アルファブレンド無効化
	glDisable(GL_BLEND);
	//デプスバッファ有効化
	glEnable(GL_DEPTH_TEST);
	//航空機の軌道の描画完了

	//クリッピング平面の無効化
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glDisable(GL_CLIP_PLANE2);
	glDisable(GL_CLIP_PLANE3);
	
	//オブジェクト描画コマンドを発行する
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
		//camera_target[0]+=1.0;
		center_offset_long += 1.0 / scale;
		display();
		break;
	case 's':
		//camera_target[0]-=1.0;
		center_offset_long -= 1.0 / scale;
		display();
		break;
	case 'e':
		//camera_target[1]+=1.0;
		center_offset_lat += 1.0 / scale;
		display();
		break;
	case 'd':
		//camera_target[1]-=1.0;
		center_offset_lat -= 1.0 / scale;
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
		//camera_r/=0.875;
		//camera_r = (camera_r > 10000.0) ? 10000.0 : camera_r;
		scale*=0.875;
		display();
		break;
	case ' ':
		//camera_r*=0.875;
		//camera_r = (camera_r < 1.0) ? 1.0 : camera_r;
		scale/=0.875;
		display();
		break;
	case 't':
		now+=10;
		if (now > 1453270000)
		{
			now = 1453260000;
		}
		display();
		break;
	}
}

void initPathPoint()
{
	
	cout<<"dba() before"<<endl;
	DBAccessor dba(std::string("../../db/ATFViewer.db"));
	cout<<"dba() after, setQuery() before"<<endl;
	//dba.setQuery(std::string("select longitude,latitude,altitude,time from TrackData where id='895024a' order by time;"));
	//1時間分の軌道を取得する
	dba.setQuery(std::string("select id,longitude,latitude,altitude,time from TrackData where time>=1453260000 and time<1453270000 order by id,time;"));
	cout<<"setQuery() after, step_select() before"<<endl;
	
	now = 1453260000;
	
	//パスのインデックス
	int n=-1;
	//直前に読み込んだid
	//id毎にvectorに格納するため
	std::string old_id("");
	
	while(SQLITE_ROW == dba.step_select())
	{
		std::string id(dba.getColumnString(0));
		double lo=dba.getColumnDouble(1);
		double la=dba.getColumnDouble(2);
		int a=dba.getColumnInt(3);
		long long t=dba.getColumnLongLong(4);
		if(id != old_id)
		{
			n++;
			paths.push_back(vector<PathPoint>());
			old_id = id;
		}
		paths[n].push_back(PathPoint(lo,la,a,t));
	}
	cout<<"initPathPoint() end"<<endl;
}

void init(void)
{
	
	initPathPoint();
	
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

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	
	//デプスバッファを使用する
	glEnable(GL_DEPTH_TEST);
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	init();
	glutMainLoop();
	
	return 0;
}

*/

#include "ATFViewerMain.h"

int main(int argc, char const* argv[])
{
	ATFViewerMain& a = ATFViewerMain::getInstance();
	a.init();
	a.execMainLoop();
	return 0;
}

