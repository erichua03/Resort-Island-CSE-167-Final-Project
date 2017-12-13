#ifndef _BEZIER_H_
#define _BEZIER_H_
#include "Window.h"
#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <list>
#include <vector>
#include <array>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Node {
public:
	virtual void draw(glm::mat4 C) = 0;
	virtual void update(glm::vec3 point[]) = 0;
};

class Transform : public Node {
public:

	glm::mat4 M=glm::mat4(1.0f);
	Transform(glm::mat4 toW);
	std::list<Node*> children;
	void draw(glm::mat4 C);
	void update(glm::vec3 point[]);
	void addChild(Node* child);
	void removeChild(Node* toD);
};

class Geometry : public Node {
public:
	int acc = 0;
    int flag;
	//float bazierCurve[441 * 3] = {};
    //unsigned int indices[400*3*2];
    std::vector<float> bazierCurve;
    std::vector<unsigned int> indices;
    std::vector<float> normals;
	GLuint VBO=0, VAO=0, EBO=0;
	glm::vec3 ctrlPoints[16] = {};
	Geometry(glm::vec3 point[],float,float,float);
    Geometry(std::vector<glm::vec3>, std::vector<unsigned>,float,float,float);
	void draw(glm::mat4 C);
	void update(glm::vec3 point[]);
	int typeR;
    float r,g,b;
};
#endif
