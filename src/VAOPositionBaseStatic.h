/*
 * =====================================================================================
 *
 *       Filename:  VAOPositionBaseStatic.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月29日 03時02分58秒
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
using VAOPositionBaseStatic = VAOPositionBase<S, VBOStatic, VBOElementStatic, VAOLayoutClass>;


