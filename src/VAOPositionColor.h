/*
 * =====================================================================================
 *
 *       Filename:  VAOPositionColor.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月31日 19時48分15秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include "VAOPositionBase.h"



template <typename S, typename VBOClass, typename VBOElementClass>
using VAOPositionColorBase = VAOPositionBase<S, VBOClass, VBOElementClass, VAOLayoutPositionColor>;

template <typename S>
using VAOPositionColor = VAOPositionColorBase<S, VBOStatic, VBOElementStatic>;

template <typename S>
class VAOPositionColorDynamic
{
private:
	using baseType = VAOPositionColorBase<S, VBODynamic, VBOElementDynamic>;
	baseType base;

public:
	
	using inputType = typename baseType::inputType;
	
	VAOPositionColorDynamic(S& s):base(s)
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
	void initReady(const inputType& v, const std::vector<unsigned int>& e, GLenum m)
	{
		base.initReady(v, e, m);
	}
	void initMain()
	{
		base.initMain();
	}
	void init(const inputType& v, const std::vector<unsigned int>& e, GLenum m)
	{
		base.init(v, e, m);
	}
	void display()
	{
		base.display();
	}
	void map()
	{
		base.getPosition().map();
		base.getColor().map();
		base.getElement().map();
	}
	void unmap()
	{
		base.getPosition().unmap();
		base.getColor().unmap();
		base.getElement().unmap();
	}
	float* getPositionDevicePointer()
	{
		return base.getPosition().getDevicePointer();
	}
	float* getColorDevicePointer()
	{
		return base.getColor().getDevicePointer();
	}
	unsigned int* getElementDevicePointer()
	{
		return base.getElement().getDevicePointer();
	}
	int getVertexCount()
	{
		return base.getVertexCount();
	}
};


