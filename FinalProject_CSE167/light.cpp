#include "light.h"
#include "Window.h"
#include <iostream>


using namespace std;



light::light(int mode)
{
	light_mode = mode;
	light_pos = glm::vec3(0.0f, 0.0f, 5.0f);
	light_color = glm::vec3(0.7f, 0.7f, 0.7f);
	light_dir = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
	linear_att = 0.1f;
	quad_att = 0.001f;
	cutoff_angle = 0.01f;
	exponent = 20.0f;
	lightToWorld = glm::mat4(1.0f);
}


light::~light()
{


}

void light::draw(GLuint shaderProgram) {
	ulightMode = glGetUniformLocation(shaderProgram, "light.light_mode");
	glUniform1i(ulightMode, light_mode);
	color = glGetUniformLocation(shaderProgram, "light.light_color");
	glUniform3f(color, light_color.x, light_color.y, light_color.z);

	glm::mat4 view = Window::V * lightToWorld;
	GLuint transform = glGetUniformLocation(shaderProgram, "light.transform");
	glUniformMatrix4fv(transform, 1, GL_FALSE, &lightToWorld[0][0]);
	// direction light 
	if (light_mode == 0) {
		//std::cout << "DIRECTION" << std::endl;
		direction = glGetUniformLocation(shaderProgram, "light.light_dir");
		glUniform3f(direction, light_dir.x, light_dir.y, light_dir.z);
	}

	// point light
	else if (light_mode == 1) {
		//std::cout << "POINT" << std::endl;
		position = glGetUniformLocation(shaderProgram, "light.light_pos");
		linear = glGetUniformLocation(shaderProgram, "light.linear_att");
		glUniform3f(position, light_pos.x, light_pos.y, light_pos.z);
		glUniform1f(linear, linear_att);
	}

	else if (light_mode == 2) {
		//std::cout << "spot Light" << std::endl;
		direction = glGetUniformLocation(shaderProgram, "light.light_dir");
		glUniform3f(direction, light_dir.x, light_dir.y, light_dir.z);

		position = glGetUniformLocation(shaderProgram, "light.light_pos");
		glUniform3f(position, light_pos.x, light_pos.y, light_pos.z);
		
		quadratic = glGetUniformLocation(shaderProgram, "light.quad_att");
		cutoff = glGetUniformLocation(shaderProgram, "light.cutoff_angle");
		uExponent = glGetUniformLocation(shaderProgram, "light.exponent");
		glUniform1f(quadratic, quad_att);
		glUniform1f(cutoff, cutoff_angle);
		glUniform1f(uExponent, exponent);
	}
}
