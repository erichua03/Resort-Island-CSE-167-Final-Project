#ifndef GEOMETRY_H
#define GEOMETRY_H

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
#include "light.h"
#include "Node.h"
#include <vector>

using namespace std;

class Geometry : public Node
{
public:
	std::vector<GLuint> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	glm::mat4 toWorld;
	float size;

	float avgX, avgY, avgZ;

	Geometry(const char *filepath);
	~Geometry();
	Geometry(const char * filePath, int x);

	void parse(const char *filepath);


	std::vector<GLuint> lightIndices;
	std::vector<glm::vec3> lightVertices;

	std::vector<GLuint> lightIndices1;
	std::vector<glm::vec3> lightVertices1;

	void parseLight(const char *filepath);

	const char *cone;
	const char *sphere;
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update();
	void spin(float);
	void rotate(float deg, glm::vec3 axis);
	void translate(float x, float y, float z);
	void scale(float x, float y, float z);
	void drawLight(GLuint shaderProgram, light light);
	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO, VBO2, LVBO, LVAO, LEBO;
	GLuint uProjection, uModelview, uView, uModel, lProjection, lModelview;
	GLuint 	uDiffuse, uSpecular, uAmbient, uShininess, uRenderMode;


	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;


	int shininess;

	int renderMode = 1;
};



#endif