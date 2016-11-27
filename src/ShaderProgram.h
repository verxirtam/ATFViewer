/*
 * =====================================================================================
 *
 *       Filename:  ShaderProgram.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月31日 19時41分25秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include "OpenGLHeaders.h"

#include "Shader.h"


#include <iostream>


class ShaderProgram
{
private:
	GLuint handle;
	
	//コピー・ムーブを禁止するため下記の宣言のみ行う
	//コピーコンストラクタ
	ShaderProgram(const ShaderProgram& s) = delete;
	//コピー代入演算子
	ShaderProgram& operator=(const ShaderProgram& s) = delete;
	//ムーブコンストラクタ
	ShaderProgram(ShaderProgram&& s) = delete;
	//ムーブ代入演算子
	ShaderProgram& operator=(ShaderProgram&& s) = delete;
public:
	ShaderProgram():handle(0)
	{
	}
	~ShaderProgram()
	{
		glDeleteProgram(handle);
	}
	void init()
	{
		//Proguramオブジェクトの開放
		glDeleteProgram(handle);
		//Proguramオブジェクトの生成
		handle = glCreateProgram();
		if(handle == 0)
		{
			std::cout << "error at glCreateProgram()." << std::endl;
		}
		
	}
	
	void attach(const Shader& s)
	{
		glAttachShader(handle, s.getHandle());
	}
	void link()
	{
		glLinkProgram(handle);
		
		//リンクの結果確認
		GLint status;
		glGetProgramiv(handle, GL_LINK_STATUS, &status);
		if(status == GL_FALSE)
		{
			std::cout << "error at glLinkProgram(handle)." << std::endl;
		}
	}
	void use()
	{
		//OpenGLパイプラインにインストール
		glUseProgram(handle);
	}
	void unuse()
	{
		//OpenGLパイプラインプログラムを割り当てない
		glUseProgram(0);
	}
	GLuint getHandle()
	{
		return handle;
	}
};
