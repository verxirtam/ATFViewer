/*
 * =====================================================================================
 *
 *       Filename:  UniformVariable.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月31日 19時38分06秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include <string>

#include "OpenGLHeaders.h"
#include <glm/gtc/type_ptr.hpp>

template <typename T>
class UniformVariable
{
private:
	GLint location;
public:
	void setLocation(GLuint program_handle, const char* name)
	{
		GL_CALL(location = glGetUniformLocation(program_handle, name));
	}
	void setLocation(GLuint program_handle, const std::string& name)
	{
		GL_CALL(location = glGetUniformLocation(program_handle, name.c_str()));
	}
	void set(const T& t);
};

template<>
void UniformVariable<glm::mat4>::set(const glm::mat4& m);

template<>
void UniformVariable<glm::vec3>::set(const glm::vec3& v);

template<>
void UniformVariable<int>::set(const int& i);

