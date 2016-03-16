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


#ifndef GCS_H_
#define GCS_H_

#include <vector>

struct LongLat
{
	double longitude;
	double latitude;
};


//頂点配列が右回り(時計回り)かどうかを判定する
bool isClockwise(std::vector<int>& longLatIndex, std::vector<LongLat>& longLat);

#endif
