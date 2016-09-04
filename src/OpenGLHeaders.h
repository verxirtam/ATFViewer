/*
 * =====================================================================================
 *
 *       Filename:  OpenGLHeaders.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月14日 00時16分22秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#pragma once


#include <GL/glew.h>
#include <GL/glut.h>

//nvcc実行時はGLM_FORCE_CUDAを定義
//-> GLM 0.9.6.0 で削除されていたので不要
//#ifdef __CUDACC__
//#ifdef __NVCC__
//#define GLM_FORCE_CUDA
//#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

