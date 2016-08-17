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
	VAOBase(const VAOBase& v);
	VAOBase& operator=(const VAOBase& v);
	VAOBase(VAOBase&& v);
	VAOBase& operator=(VAOBase&& v);
public:
	VAOBase():handle(0)
	{
	}
	~VAOBase()
	{
		//VAOの開放
		glDeleteVertexArrays(1, &handle);
	}
	void bind()
	{
		glBindVertexArray(handle);
	}
	void unbind()
	{
		glBindVertexArray(0);
	}
	void init()
	{
		//VAOの開放
		//handle!=0の時のために実行する
		glDeleteVertexArrays(1, &handle);
		
		//VAOの確保
		glGenVertexArrays(1, &handle);
		
	}
};


