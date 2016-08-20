/*
 * =====================================================================================
 *
 *       Filename:  GCS.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年03月16日 22時38分51秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "GCS.h"


//頂点配列が右回り(時計回り)かどうかを判定する
bool isClockwise(const std::vector<unsigned int>& longLatIndex, const std::vector<LongLat>& longLat)
{
	//頂点列の作る多角形の面積の2倍
	double s = 0.0;
	//頂点配列の長さ
	int length = longLatIndex.size();
	//長さが2未満の時は半時計回りとみなす
	if(length < 2)
	{
		return false;
	}
	int lli  = 0;
	int lli1 = 0;
	//頂点列の作る多角形の面積を求める
	for(int i = 0; i < length -1; i++)
	{
		lli  = longLatIndex[i    ];
		lli1 = longLatIndex[i + 1];
		s += longLat[lli].longitude * longLat[lli1].latitude - longLat[lli1].longitude * longLat[lli].latitude;
	}
	lli  = longLatIndex[length -1];
	lli1 = longLatIndex[0        ];
	s += longLat[lli].longitude * longLat[lli1].latitude - longLat[lli1].longitude * longLat[lli].latitude;
	
	//面積が正なら半時計回り
	if(s >=  0.0)
	{
		return false;
	}
	//面積が負なら時計回り
	return true;
}

