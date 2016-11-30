/*
 * =====================================================================================
 *
 *       Filename:  StringVAO.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年11月28日 01時30分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Texture2D.h"
#include "VAOPositionTexture.h"
#include "ShaderProgramTexture.h"



class StringVAO
{
private:
	Texture2D texture;
	ShaderProgramTexture shaderProgram;
	using vaoType = VAOPositionTexture<ShaderProgramTexture>;
	vaoType vao;
	
	void getTextureCoordinate(int char_code, float& umin, float& vmin, float& umax, float& vmax)
	{
		int n = char_code - 32;
		int i = (n % 16) * 32;
		int j = (n / 16) * 64;
		umin = static_cast<float>(i     ) / 512.0f;
		vmin = static_cast<float>(j     ) / 512.0f;
		umax = static_cast<float>(i + 32) / 512.0f;
		vmax = static_cast<float>(j + 64) / 512.0f;
		
	}
	
public:
	StringVAO():
		texture(GL_TEXTURE0),
		shaderProgram(),
		vao(shaderProgram, texture)
	{
	}
	void init()
	{
		std::string disp_string("test.");
		
		
		shaderProgram.init();
		glm::mat4 m;
		m = glm::scale(m, glm::vec3(0.125f, 0.125f, 0.125f));
		shaderProgram.setMVPMatrix(m);
		
		texture.init("../../res/font/ExportedFont_Ricty_64_32.raw", 512, 512);
		
		vaoType::inputType input;
		std::vector<unsigned int> e;
		
		unsigned int nmax = disp_string.length();
		for (unsigned int n = 0; n < nmax; n++)
		{
			
			std::vector<float> add_position
				{
					0.5f *  n     , 1.0f, 0.0f,//4 * n
					0.5f *  n     , 0.0f, 0.0f,//4 * n + 1
					0.5f * (n + 1), 1.0f, 0.0f,//4 * n + 2
					0.5f * (n + 1), 0.0f, 0.0f //4 * n + 3
				};
			input.position.insert(input.position.end(), add_position.begin(), add_position.end());
			
			std::vector<unsigned int> add_element
				{
					4 * n,
					4 * n + 1,
					4 * n + 2,
					4 * n + 2,
					4 * n + 1,
					4 * n + 3,
				};
			e.insert(e.end(), add_element.begin(), add_element.end());
			
			float u0, v0;
			float u1, v1;
			
			getTextureCoordinate(disp_string[n], u0, v0, u1, v1);
			
			std::vector<float> add_texture
				{
					u0, v0,
					u0, v1,
					u1, v0,
					u1, v1
				};
			input.texture.insert(input.texture.end(), add_texture.begin(), add_texture.end());
		}
		vao.init(input, e, GL_TRIANGLES);
	}
	void display()
	{
		vao.display();
	}
};


