/*
 * =====================================================================================
 *
 *       Filename:  UniformVariable.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月31日 20時40分00秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "UniformVariable.h"

template<>
void UniformVariable<glm::mat4>::set(const glm::mat4& m)
{
	GL_CALL(glUniformMatrix4fv(this->location, 1, GL_FALSE, &m[0][0]));
}

template<>
void UniformVariable<glm::vec3>::set(const glm::vec3& v)
{
	GL_CALL(glUniform3fv(this->location, 1, glm::value_ptr(v)));
}

template<>
void UniformVariable<int>::set(const int& i)
{
	GL_CALL(glUniform1i(this->location, static_cast<GLint>(i)));
}

