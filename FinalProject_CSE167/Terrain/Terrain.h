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
    
    vector<glm::vec3> tangents; // binded with VBO_T
    vector<glm::vec3> bitangents; // binded with VBO_BT
    
    vector<glm::vec2> uvs; // texture coordinates
    // texCoords_2D = vec2(position.x, position.z);
    
public:
    
    GLuint VAO, VBO, EBO, NBO; // vertices, indices, normals
    GLuint VBO_T, VBO_BT; // tangent and bitangent
    vector<float> yVal; // heights
    vector<float> hMap; // terrain heights
    
    Terrain();
    ~Terrain();
    
    GLuint rockTexture, grassTexture, snowTexture, normalTexture; // textureIDs
    static const unsigned int m_uiNumTextures = 3;
    GLuint  m_GLTextures[m_uiNumTextures];
    
    static float minX;
    static float minY;
    static float minZ;
    
    static float maxX;
    static float maxY;
    static float maxZ;

    void draw(GLuint shaderProgram, glm::mat4 toWorld);
    GLuint loadTexture(const char* textureFile);
    
    float getHeight(int x, int z, int height);//
    void generateHeightMap(unsigned int width, unsigned int height);
    
    void generateVertices();
    void generateIndices();
    void generateNormals();
    
    void generateUVs();
    
    void generateTangents(); // and bitangents
    void gen_tangent_bitangent(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec2 uv0, glm::vec2 uv1, glm::vec2 uv2);
};

#endif /* Terrain_h */
