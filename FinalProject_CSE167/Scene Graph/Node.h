#ifndef _NODE_H_
#define _NODE_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Node{
	public:
		glm::mat4 M;
		virtual void draw(GLuint shaderProgram,glm::mat4 C) = 0;
		virtual void update() = 0;
		virtual void translate(float x,float y, float z) = 0;
};
#endif