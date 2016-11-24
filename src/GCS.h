/*
 * =====================================================================================
 *
 *       Filename:  GCS.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年03月16日 21時57分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#pragma once

#include <vector>

#include "LongLat.h"

//頂点配列が右回り(時計回り)かどうかを判定する
bool isClockwise(const std::vector<unsigned int>& longLatIndex, const std::vector<LongLat>& longLat);

