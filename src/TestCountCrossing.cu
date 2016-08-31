/*
 * =====================================================================================
 *
 *       Filename:  TestCountCrossing.cu
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年06月08日 16時58分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include "TestCountCrossing.h"
#include "CountCrossing.cuh"
/*
#define SWITCH_D(F,ARG,D) \
	switch ((D))\
	{\
	case 1:\
		(F)<1>ARG;\
		break;\
	case 2:\
		(F)<2>ARG;\
		break;\
	case 3:\
		(F)<3>ARG;\
		break;\
	case 4:\
		(F)<4>ARG;\
		break;\
	}

#define SWITCH_D_0_CASE(F,ARG,D) \
		case 0:\
			return (F)<(D),0>ARG;\
			break;

#define SWITCH_D_1_CASE(F,ARG,D) \
	SWITCH_D_0_CASE(F,ARG,D)\
		case 1:\
			return (F)<(D),1>ARG;\
			break;

#define SWITCH_D_2_CASE(F,ARG,D) \
	SWITCH_D_1_CASE(F,ARG,D)\
		case 2:\
			return (F)<(D),2>ARG;\
			break;


#define SWITCH_D_3_CASE(F,ARG,D) \
	SWITCH_D_2_CASE(F,ARG,D)\
		case 3:\
			return (F)<(D),3>ARG;\
			break;


#define SWITCH_D_DI(F,ARG,D,DI) \
	switch ((D))\
	{\
	case 1:\
		switch((DI))\
		{\
			SWITCH_D_0_CASE(F,ARG,D)\
		}\
		break;\
	case 2:\
		switch((DI))\
		{\
			SWITCH_D_1_CASE(F,ARG,D)\
		}\
		break;\
	case 3:\
		switch((DI))\
		{\
			SWITCH_D_2_CASE(F,ARG,D)\
		}\
		break;\
	case 4:\
		switch((DI))\
		{\
			SWITCH_D_3_CASE(F,ARG,D)\
		}\
		break;\
	}


float*  TestCountCrossing::getCrossingPoint
	(
		int dimension,				//次元
		int directionindex,			//方向
		const float* const start,		//線分の始点
		const float* const end,			//線分の終点
		float x,			//交点を求める平面の座標
		float* const cross		//交点の出力先
	)
{
	SWITCH_D_DI(getCrossingPoint, (start,end,x,cross), dimension, directionindex)
}
*/
/*
template <typename RET_TYPE, template< template<int, int> F> class FUNC, typename ARG0, typename ARG1, typename ARG2, typename ARG3>
RET_TYPE testFunction
	(
		int dimension,				//次元
		int directionindex,			//方向
		FUNC<> function,
		ARG0 arg0,
		ARG1 arg1,
		ARG2 arg2,
		ARG3 arg3
	)
{
	RET_TYPE ret;
	ret = function<0,1>(arg0,arg1,arg2,arg3);
	return ret;
}
*/

template<int D, int DI>
float*  TestCountCrossing::getCrossingPoint
	(
		const float* const start,		//線分の始点
		const float* const end,			//線分の終点
		float x,			//交点を求める平面の座標
		float* const cross		//交点の出力先
	)
{
	return ::getCrossingPoint<D, DI>( start, end, x, cross);
/*
	float* ret;
	switch (dimension)
	{
	case 1:
		switch (directionindex)
		{
		case 0:
			ret = ::getCrossingPoint<1,0>(start,end,x,cross);
			break;
		}
		break;
	case 2:
		switch (directionindex)
		{
		case 0:
			ret = ::getCrossingPoint<2,0>(start,end,x,cross);
			break;
		case 1:
			ret = ::getCrossingPoint<2,1>(start,end,x,cross);
			break;
		}
		break;
	case 3:
		switch (directionindex)
		{
		case 0:
			ret = ::getCrossingPoint<3,0>(start,end,x,cross);
			break;
		case 1:
			ret = ::getCrossingPoint<3,1>(start,end,x,cross);
			break;
		case 2:
			ret = ::getCrossingPoint<3,2>(start,end,x,cross);
			break;
		}
		break;
	case 4:
		switch (directionindex)
		{
		case 0:
			ret = ::getCrossingPoint<4,0>(start,end,x,cross);
			break;
		case 1:
			ret = ::getCrossingPoint<4,1>(start,end,x,cross);
			break;
		case 2:
			ret = ::getCrossingPoint<4,2>(start,end,x,cross);
			break;
		case 3:
			ret = ::getCrossingPoint<4,3>(start,end,x,cross);
			break;
		}
		break;
	}
	return ret;
*/
}

