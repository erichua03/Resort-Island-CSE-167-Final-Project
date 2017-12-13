#ifndef _PLANT_H_
#define _PLANT_H_

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
#include <stdio.h>
#include <vector>


class Plant
{
public:
    Plant();
    ~Plant();
    float angleA;
    float angleB;
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> vertices;
    
    GLuint VBO, VAO,EBO;
    GLuint uProjection, uModelview;
    glm::mat4 toWorld;
    
    void draw(GLuint shaderProgram, int n, glm::mat4 trans, glm::vec3 start, char input, std::string rules[]);
    void parse(const char *filepath);
    void update(int i);
};

#endif

