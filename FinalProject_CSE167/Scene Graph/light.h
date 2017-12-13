#ifndef LIGHT_H
#define LIGHT_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#include <algorithm>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

using namespace std;
class light
{
public:
	
	light(int mode);
	~light();

	int light_mode;
	glm::vec3 light_color;
	glm::vec3 light_pos;
	glm::vec3 light_dir;
	float cons_att;
	float linear_att;
	float quad_att;
	float cutoff_angle;
	float exponent;	glm::mat4 lightToWorld;	void draw(GLuint shaderProgram);	GLuint color, direction, position, linear, quadratic, cutoff, uExponent, ulightMode;
};

#endif
