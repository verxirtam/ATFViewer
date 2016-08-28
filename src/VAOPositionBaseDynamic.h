/*
 * =====================================================================================
 *
 *       Filename:  VAOPositionBaseDynamic.h
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

template <typename S, typename VAOLayoutClass>
class VAOPositionBaseDynamic
{
private:
	using baseType = VAOPositionBase<S, VBODynamic, VBOElementDynamic, VAOLayoutClass>;
	baseType base;

public:
	
	using inputType = typename baseType::inputType;
	
	VAOPositionBaseDynamic(S& s):base(s)
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
		base.getVertex().map();
		base.getElement().map();
	}
	void unmap()
	{
		base.getVertex().unmap();
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


