//
//  Terrain.hpp
//  FinalProject_CSE167
//
//  Created by Brittany Zhang on 12/5/17.
//  Copyright © 2017 ManianaBonita. All rights reserved.
//

#ifndef Terrain_h
#define Terrain_h

#include <iostream>
#include <stdio.h>
#include <vector>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "stb_image.h"

// Use of degrees is deprecated. Use radians instead.
// Include lines below for compiler to recognize glm functions
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdio.h>

using namespace std;

class Terrain {
protected:
    vector<glm::vec3> vertices; // binded with VBO
    vector<GLuint> indices; // binded with EBO
    vector<glm::vec3> normals; // binded with NBO
    
public:
    
    GLuint VAO, VBO, EBO, NBO;
    GLuint rockTexture, grassTexture, snowTexture; // textureIDs
    vector<float> yVal; // heights
    vector<float> hMap; // terrain heights
    
    Terrain();
    ~Terrain();

    void draw(GLuint shaderProgram, glm::mat4 toWorld);
    void loadTexture(const char* textureFile, GLuint textureID);
    float getHeight(int x, int z, int height);//
    void generateHeightMap(unsigned int width, unsigned int height);
    
    void generateVertices();
    void generateIndices();
    void generateVertices(unsigned int width, unsigned int height);
    void generateIndices(vector<glm::vec3> vertices, int width, int height); // unsigned?
    void generateNormals(vector<glm::vec3> vertices, unsigned int width, unsigned int height); //
};

#endif /* Terrain_h */
