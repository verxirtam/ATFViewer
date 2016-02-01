/*
 * =====================================================================================
 *
 *       Filename:  ATFViewerMain.cpp
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
#include "ATFViewerMain.h"

void ATFViewerMain::initScene(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//テクスチャの読み込み
	//テクスチャ読み込み用の配列
	const int TEXWIDTH = 600;
	const int TEXHEIGHT = 600;
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

	//デプスバッファを使用する
	glEnable(GL_DEPTH_TEST);
}


void ATFViewerMain::display(void)
{
	
}

void ATFViewerMain::resize(void)
{
	
}

void ATFViewerMain::keyboard(unsigned char key, int x, int y)
{
	
}




