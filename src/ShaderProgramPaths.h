/*
 * =====================================================================================
 *
 *       Filename:  ShaderProgramPaths.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月31日 19時44分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include "ShaderProgramBaseMVPMatrix.h"


struct ShaderVertPathPaths
{
	static std::string getPath(){return "paths.vert";}
};

struct ShaderFragPathPaths
{
	static std::string getPath(){return "paths.frag";}
};

using ShaderProgramPaths = ShaderProgramBaseMVPMatrix<ShaderVertPathPaths, ShaderFragPathPaths>;


