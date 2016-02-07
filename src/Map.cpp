/*
 * =====================================================================================
 *
 *       Filename:  Map.cpp
 *
 *    Description:  マップを描画する
 *
 *        Version:  1.0
 *        Created:  2016年02月07日 17時27分01秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "Map.h"

void Map::init(void)
{
	//テクスチャの読み込み
	//テクスチャ読み込み用の配列
	const int TEXWIDTH = 2048;//600;
	const int TEXHEIGHT = 2048;//600;
	GLubyte* texture = (GLubyte*) std::malloc( sizeof(GLubyte) * TEXWIDTH * TEXHEIGHT * 3);
	//GLubyte texture[TEXWIDTH * TEXHEIGHT * 3];
	/*
	GLubyte*** texture;
	texture = new GLubyte**[TEXWIDTH];
	for (int  i = 0; i < TEXWIDTH; i++)
	{
		texture[i] = new GLubyte*[TEXHEIGHT];
		for (int j = 0; j < TEXHEIGHT; j++)
		{
			texture[i][j] = new GLubyte[3];
		}
	}
	*/
	//テクスチャファイルのファイルポインタ
	std::FILE* fp;
	//テクスチャファイル名
	//const char texture1[]="/home/daisuke/programs/ATFViewer/res/el_v2_0600_090-180_00-90.raw";
	const char texture1[]="/home/daisuke/programs/ATFViewer/res/JP-ENR-6.1-en-JP-1_test_2048.raw";
	//テクスチャファイルの読み込み
	if ((fp = std::fopen(texture1, "rb")) != NULL)
	{
		//std::fread(texture, sizeof(texture),1,fp);
		std::fread(texture, sizeof(GLubyte) * TEXWIDTH * TEXHEIGHT * 3,1,fp);
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
	
	
	std::free(texture);
	/*
	//動的に確保した配列の開放
	for (int  i = 0; i < TEXWIDTH; i++)
	{
		for (int j = 0; j < TEXHEIGHT; j++)
		{
			delete[] texture[i][j];
			texture[i][j] = NULL;
		}
		delete[] texture[i];
		texture[i] = NULL;
	}
	delete[] texture;
	texture = NULL;
	*/
}
void Map::display(void)
{
	//モデリング変換行列の設定
	//モデル座標は経緯度-feetのサイズになっているので
	//地図っぽくスケーリングする
	//経緯度はそのままで行けそうなので、feetのみ変更
	//40000feetが4cmになるように1/10000に縮小する
	glScaled(scale,scale,0.0001);
	//羽田+offsetがワールド座標系の原点に来るように平行移動する
	double haneda_x = 139.0 + ( 46.0/60.0 + 87.0/3600.0 );
	double haneda_y =  35.0 + ( 33.0/60.0 + 20.0/3600.0 );
	glTranslated( - haneda_x - centerOffsetLong, - haneda_y - centerOffsetLat, 0.0);
	
	//世界地図を描く
	glColor3d(1.0,1.0,1.0);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	/*
	glNormal3d(0.0,0.0,1.0);
	glTexCoord2d(0.0,0.0);glVertex3d( 90.0,90.0,0.0);//-180.0, 90.0,0.0);
	glTexCoord2d(0.0,1.0);glVertex3d( 90.0, 0.0,0.0);//-180.0,-90.0,0.0);
	glTexCoord2d(1.0,0.0);glVertex3d(180.0,90.0,0.0);// 180.0, 90.0,0.0);
	glNormal3d(0.0,0.0,1.0);
	glTexCoord2d(0.0,1.0);glVertex3d( 90.0, 0.0,0.0);//-180.0,-90.0,0.0);
	glTexCoord2d(1.0,1.0);glVertex3d(180.0, 0.0,0.0);// 180.0,-90.0,0.0);
	glTexCoord2d(1.0,0.0);glVertex3d(180.0,90.0,0.0);// 180.0, 90.0,0.0);
	*/
	GLdouble tex_coord[][2]={
								{ 72.2/1024.0,  71.2/1024.0},
								{991.4/1024.0,  22.0/1024.0},
								{198.2/1024.0, 893.5/1024.0},
								{981.3/1024.0, 893.1/1024.0}
							};
	GLdouble ver_coord[][3]={
								{138.0, 34.0 + 58.0/60.0, 0.0},
								{140.0, 38.0 + 30.0/60.0, 0.0},
								{142.0, 33.0 + 48.0/60.0, 0.0},
								{144.0, 36.0 + 40.0/60.0, 0.0}
							};
	//0,2,1   1,2,3
	glTexCoord2d(tex_coord[0][0],tex_coord[0][1]);glVertex3d(ver_coord[0][0],ver_coord[0][1],ver_coord[0][2]);
	glTexCoord2d(tex_coord[1][0],tex_coord[1][1]);glVertex3d(ver_coord[1][0],ver_coord[1][1],ver_coord[1][2]);
	glTexCoord2d(tex_coord[2][0],tex_coord[2][1]);glVertex3d(ver_coord[2][0],ver_coord[2][1],ver_coord[2][2]);
	
	glTexCoord2d(tex_coord[1][0],tex_coord[1][1]);glVertex3d(ver_coord[1][0],ver_coord[1][1],ver_coord[1][2]);
	glTexCoord2d(tex_coord[2][0],tex_coord[2][1]);glVertex3d(ver_coord[2][0],ver_coord[2][1],ver_coord[2][2]);
	glTexCoord2d(tex_coord[3][0],tex_coord[3][1]);glVertex3d(ver_coord[3][0],ver_coord[3][1],ver_coord[3][2]);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//罫線を描画する
	glBegin(GL_LINES);
	glColor3d(1.0,0.0,0.0);
	glVertex3d(140.0,30.0,10.0);
	glVertex3d(140.0,50.0,10.0);
	glEnd();
}

