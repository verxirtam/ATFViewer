/*
 * =====================================================================================
 *
 *       Filename:  TestVBO.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月12日 01時17分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "TestVBO.h"

void TestVBO::init()
{
	float cx = 139.0f;
	float cy = 35.0f;
	float cz = 30000.0f;
	float v[8][3]
		= {
			{cx - 1.0f, cy - 1.0f, cz},
			{cx - 1.0f, cy + 1.0f, cz},
			{cx + 1.0f, cy - 1.0f, cz},
			{cx + 1.0f, cy + 1.0f, cz},
			{cx - 1.0f, cy - 1.0f, cz + 20000.0f},
			{cx - 1.0f, cy + 1.0f, cz + 20000.0f},
			{cx + 1.0f, cy - 1.0f, cz + 20000.0f},
			{cx + 1.0f, cy + 1.0f, cz + 20000.0f}
		};
	GLuint size = 8 * 3 * sizeof(float);
	
	
	
	//VAOの確保
	glGenVertexArrays(1, &vao);
	//VAOを設定対象にする
	glBindVertexArray(vao);
	
	//VBOの確保
	glGenBuffers(1, &vbo);
	//VBOを有効にする(指定したVBOを処理対象にする)
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//GPU上のVBOに頂点の座標を格納する
	glBufferData(GL_ARRAY_BUFFER, size, v, GL_STATIC_DRAW);
	
	//VBOをVAOに処理対象として設定する
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//VAOをシェーダーの処理対象として設定する
	glEnableVertexAttribArray(0);
	
	//VBOを設定対象から外す
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//VAOを設定対象から外す
	glBindVertexArray(0);
}

void TestVBO::display()
{
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, 0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(0);
	
	//VAOを処理対象にする
	glBindVertexArray(vao);
	//処理対象にしたオブジェクトを利用して描画を行う
	glDrawArrays(GL_POINTS, 0, 8);
	//VAOを処理対象から外す
	glBindVertexArray(0);
	
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}

