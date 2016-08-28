/*
 * =====================================================================================
 *
 *       Filename:  VAOPositionTexture.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月08日 03時49分15秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include "Lock.h"
#include "Texture2D.h"

#include "VAOPositionBase.h"
#include "VAOLayoutPositionTexture.h"

template <typename S, typename VBOClass, typename VBOElementClass>
using VAOPositionTextureBase = VAOPositionBase<S, VBOClass, VBOElementClass, VAOLayoutPositionTexture>;

template <typename S>
class VAOPositionTexture
{
private:
	
	using baseType = VAOPositionBase<S, VBOStatic, VBOElementStatic, VAOLayoutPositionTexture>;
	
	baseType base;
	Texture2D& texture;
	
public:
	
	using inputType = typename baseType::inputType;
	
	VAOPositionTexture(S& s, Texture2D& t):
		base(s),
		texture(t)
	{
	}
	~VAOPositionTexture()
	{
	}
	void bind()
	{
		base.bind();
	}
	void unbind()
	{
		base.unbind();
	}
	void initReady
		(
			const inputType& v,
			const std::vector<unsigned int>& e,
			GLenum m
		)
	{
		base.initReady(v, e, m);
	}
	void initMain()
	{
		base.initMain();
	}
	void init
		(
			const inputType& v,
			const std::vector<unsigned int>& e,
			GLenum m
		)
	{
		base.init(v, e, m);
	}
	void display(void)
	{
		//テクスチャのバインド
		Active<Texture2D> ta(texture);
		Bind<Texture2D> tb(texture);
		
		base.display();
	}
};





