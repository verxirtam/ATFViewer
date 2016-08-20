/*
 * =====================================================================================
 *
 *       Filename:  LongLat.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月20日 11時31分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once



/**
 * @brief 経緯度を格納する
 */
template <typename T>
struct LongLatBase
{
	T longitude;///< 経度
	T latitude; ///< 緯度
};

/**
 * @brief 経緯度をDoubleで格納する
 */
using LongLatD = LongLatBase<double>;
/**
 * @brief 経緯度をFloatで格納する
 */
using LongLatF = LongLatBase<float>;


/**
 * @brief 経緯度を格納する
 *
 * 後から要素の型を変更可能なようにusingにしておく
 */
using LongLat = LongLatD;
