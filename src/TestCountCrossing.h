/*
 * =====================================================================================
 *
 *       Filename:  TestCountCrossing.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年06月08日 16時57分48秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


class TestCountCrossing
{
private:
	
public:
	template <int D, int DI>
	float*  getCrossingPoint
	(
		const float* const start,		//線分の始点
		const float* const end,			//線分の終点
		float x,			//交点を求める平面の座標
		float* const cross		//交点の出力先
	);
};


template<> float*  TestCountCrossing::getCrossingPoint<1,0>(const float* const, const float* const, float, float* const);
template<> float*  TestCountCrossing::getCrossingPoint<2,0>(const float* const, const float* const, float, float* const);
template<> float*  TestCountCrossing::getCrossingPoint<2,1>(const float* const, const float* const, float, float* const);
template<> float*  TestCountCrossing::getCrossingPoint<3,0>(const float* const, const float* const, float, float* const);
template<> float*  TestCountCrossing::getCrossingPoint<3,1>(const float* const, const float* const, float, float* const);
template<> float*  TestCountCrossing::getCrossingPoint<3,2>(const float* const, const float* const, float, float* const);
template<> float*  TestCountCrossing::getCrossingPoint<4,0>(const float* const, const float* const, float, float* const);
template<> float*  TestCountCrossing::getCrossingPoint<4,1>(const float* const, const float* const, float, float* const);
template<> float*  TestCountCrossing::getCrossingPoint<4,2>(const float* const, const float* const, float, float* const);
template<> float*  TestCountCrossing::getCrossingPoint<4,3>(const float* const, const float* const, float, float* const);

