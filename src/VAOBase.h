/*
 * =====================================================================================
 *
 *       Filename:  VAOBase.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月17日 09時31分20秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#pragma once

class VAOBase
{
private:
	GLuint handle;
	
	//コピー・ムーブ禁止
	VAOBase(const VAOBase& v) = delete;
	VAOBase& operator=(const VAOBase& v) = delete;
	VAOBase(VAOBase&& v) = delete;
	VAOBase& operator=(VAOBase&& v) = delete;
public:
	VAOBase():handle(0)
	{
	}
	~VAOBase()
	{
		//VAOの開放
		GL_CALL(glDeleteVertexArrays(1, &handle));
	}
	void bind()
	{
		GL_CALL(glBindVertexArray(handle));
	}
	void unbind()
	{
		GL_CALL(glBindVertexArray(0));
	}
	void init()
	{
		//VAOの開放
		//handle!=0の時のために実行する
		GL_CALL(glDeleteVertexArrays(1, &handle));
		
		//VAOの確保
		GL_CALL(glGenVertexArrays(1, &handle));
		
	}
};


