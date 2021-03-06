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

#include "VAOPositionBaseStatic.h"
#include "VAOPositionBaseDynamic.h"
#include "VAOLayoutPositionColor.h"

template <typename S>
using VAOPositionColor = VAOPositionBaseStatic<S, VAOLayoutPositionColor>;

template <typename S>
using VAOPositionColorDynamic = VAOPositionBaseDynamic<S, VAOLayoutPositionColor>;

