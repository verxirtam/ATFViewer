/*
 * =====================================================================================
 *
 *       Filename:  StringTextureManager.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年12月04日 09時27分11秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "Texture2D.h"

class StringTextureManager
{
private:
	Texture2D texture;
	//コンストラクタ
	StringTextureManager():
		texture(GL_TEXTURE0)
	{
		texture.init("../../res/font/ExportedFont_Ricty_64_32.raw", 512, 512);
	}
	//シングルトンにするため禁止する
	//コピーコンストラクタ
	StringTextureManager(const StringTextureManager&) = delete;
	//コピー代入演算子
	StringTextureManager& operator=(const StringTextureManager&) = delete;
	//ムーブコンストラクタ
	StringTextureManager(StringTextureManager&&) = delete;
	//ムーブ代入演算子
	StringTextureManager& operator=(StringTextureManager&&) = delete;

public:
	inline static StringTextureManager& getInstance()
	{
		static StringTextureManager stm;
		return stm;
	}
	Texture2D& getTexture()
	{
		return texture;
	}
};
