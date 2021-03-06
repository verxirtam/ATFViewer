/*
 * =====================================================================================
 *
 *       Filename:  Shader.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月31日 19時35分07秒
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

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>


class Shader
{
private:
	GLuint handle;
	GLenum type;
	std::string sourcePath;//コンストラクタでコンパイルすればメンバとしては不要？
	std::string readShaderFile(const std::string& filename)
	{
		std::ifstream f(filename);
		std::string s{};
		std::stringstream ss{};
		while(std::getline(f, s))
		{
			ss << s << std::endl;
		}
		return ss.str();
	}
	//コピー・ムーブを禁止するため下記の宣言のみ行う
	//		禁止しない場合のメモ
	//		handleはコピーしないで別個に作る
	//		ほかはコピーでOK
	//		ムーブについては全部コピーでいいが、
	//		コピー元のハンドルは0にする
	//		（コピー元のShaderがデストラクタで破棄されないようにするため）
	//
	//コピーコンストラクタ
	Shader(const Shader& s) = delete;
	//コピー代入演算子
	Shader& operator=(const Shader& s) = delete;
	//ムーブコンストラクタ
	Shader(Shader&& s) = delete;
	//ムーブ代入演算子
	Shader& operator=(Shader&& s) = delete;
public:
	//デフォルトコンストラクタを作る？
	//->作らないとstdコンテナに格納できない
	//->作るには、typeとsourcePathを後から設定可能にする必要あり
	Shader(GLenum t, const char* path):handle(0),type(t),sourcePath(path)
	{
	}
	Shader(GLenum t, const std::string& path):handle(0),type(t),sourcePath(path)
	{
	}
	~Shader()
	{
		//シェーダオブジェクトを削除する
		GL_CALL(glDeleteShader(handle));
	}
	void init()
	{
		//2重に生成しないように
		//シェーダオブジェクトを削除する
		//handle==0の時は無視される
		GL_CALL(glDeleteShader(handle));
		
		//シェーダオブジェクトを生成する
		GL_CALL(handle = glCreateShader(type));
		if(handle == 0)
		{
			std::cout << "error at Shader()." << std::endl;
		}
	}
	void compile()
	{
		
		//シェーダのコードの読み込み
		std::string shader_code = readShaderFile(sourcePath.c_str());
		//シェーダのコードを配列に格納する
		const GLchar* code_array[] = {shader_code.c_str()};
		//シェーダへのソースの読み込み
		GL_CALL(glShaderSource(handle, 1, code_array, NULL));
		
		GLint result;
		
		//シェーダのコンパイル
		GL_CALL(glCompileShader(handle));
		GL_CALL(glGetShaderiv(handle, GL_COMPILE_STATUS, &result));
		if(result == GL_FALSE)
		{
			//シェーダのコンパイルエラーを表示する
			std::cout << "error at glGetShaderiv()." << std::endl;
			GLint log_len;
			GL_CALL(glGetShaderiv(handle,GL_INFO_LOG_LENGTH, &log_len));
			if(log_len > 0)
			{
				std::unique_ptr<char> log(new char[log_len]);
				GLsizei written;
				GL_CALL(glGetShaderInfoLog(handle, log_len, &written, log.get()));
				std::cout << log.get() << std::endl;
			}
		}
	}
	GLuint getHandle() const
	{
		return handle;
	}
	
};
