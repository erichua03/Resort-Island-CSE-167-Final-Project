//
//  Terrain.cpp
//  FinalProject_CSE167
//
//  Created by Brittany Zhang on 12/5/17.
//  Copyright © 2017 ManianaBonita. All rights reserved.
//

#include "Terrain.h"
#include "Window.h"

#define TERRAIN_X  300 // number of vertices along the x and z axes
#define TERRAIN_Z 300

//#define TERRAIN_X  5 // number of vertices along the x and z axes
//#define TERRAIN_Z 5

// the number of height nodes we have
#define TERRAIN_VERTEXCOUNT (TERRAIN_X * TERRAIN_Z)
// number of quads multiplied by 2 (as we need 2 triangles to make up each quad)
#define TERRAIN_TRIANGLECOUNT ((TERRAIN_X - 1) * (TERRAIN_Z - 1) * 2)

// The size (x and z axes) and height (y axis) for the terrain
#define TERRAIN_SIZE  300.0f
//#define TERRAIN_SIZE  5.0f
#define TERRAIN_HEIGHT 10

float Terrain::minX = numeric_limits<float>::max();
float Terrain::minY = numeric_limits<float>::max();
float Terrain::minZ = numeric_limits<float>::max();
float Terrain::maxX = numeric_limits<float>::min();
float Terrain::maxY = numeric_limits<float>::min();
float Terrain::maxZ = numeric_limits<float>::min();

int width, height, nrChannels;

// an array of floats to hold the heights of each vertex
float terrainHeights[TERRAIN_X][TERRAIN_Z];

const int SCALE = 60.0f;
const char *noise1 = "noise1.ppm";
const char *noise2 = "noise2.ppm";
const char *rockImg = "rock.ppm";
const char *grassImg = "grass.ppm";

float Terrain::getHeight(int x, int z, int height) {
    if ((float)x + (float)z * (float)height > yVal.size()-1) return 0.0f;
    return yVal[(float)x + (float)z * (float)height] * SCALE;
}

void Terrain::generateHeightMap(unsigned int width, unsigned int height) {
    int count = 0;
    hMap = yVal;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (count < yVal.size()) {
                hMap[i * width + j] = yVal[count];
                count++;
            }
        }
    }
}

Terrain::Terrain() {

//    int width, height, nrChannels;
    unsigned char *rgb = stbi_load(noise1, &width, &height, &nrChannels, 0); // height map
    if (!rgb) cout << "Image failed to load at path: " << noise1 << endl;
    // rgb is now three bytes per pixel, width * height size. Or NULL if load failed.
    
    for (size_t i = 0; i < width * height * 3; i += 3) {
        yVal.push_back((float)rgb[i]/255.0f);
    }
    stbi_image_free(rgb);
    
    // push empty value as place holders into vertices and indices vector
    for (int x = 0; x < TERRAIN_VERTEXCOUNT; x++) {
        vertices.push_back(glm::vec3(1.0f));
    }
//    for (int x = 0; x < TERRAIN_TRIANGLECOUNT; x++) {
//        indices.push_back(0);
//    }
    
//    for (int x = 0; x < (TERRAIN_X-1) * (TERRAIN_Z-1) * 6; x++) {
//        indices.push_back(0);
//    }


    generateHeightMap(width, height);
    generateVertices();
    generateIndices();
    generateNormals();
    
//    for (glm::vec3 n: normals) {
//        cout << "normal: " << n.x << ", " << n.y << ", " << n.z << endl;
//    }
    
    ///////////////////////////////////
    
    loadTexture(rockImg, rockTexture);
//    loadTexture(grassImg, grassTexture);
    
    // Binding data
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &NBO); // normal coloring
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat) * 3, vertices.data(), GL_STATIC_DRAW); // &vertices[0].x
    
    // Enable the usage of layout location 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat) * 3, normals.data(), GL_STATIC_DRAW); // sizeof(glm::vec3)
    
    // Enable the usage of layout location 1
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); // when layout=1
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &(indices[0]), GL_STATIC_DRAW);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint) * 3, &indices[0][0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}

unsigned char* loadTexture(const char *textureFile) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(textureFile, &width, &height, &nrChannels, 0);
    if (!data) cout << "Image failed to load at path: " << textureFile << endl;
    return data;
}

void Terrain::loadTexture(const char *textureFile, GLuint textureID) {
//    // bind textures
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // load texture image
    int width, height, nrChannels;
    unsigned char *data = stbi_load(textureFile, &width, &height, &nrChannels, 0);
    if (!data) cout << "Image failed to load at path: " << rockImg << endl;

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // mipmap linear?

    stbi_image_free(data);
}

Terrain::~Terrain() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &NBO);
}

void Terrain::generateVertices() {
    float fTerrainZ, fTerrainX;
    fTerrainZ = TERRAIN_SIZE/2;
    for (int z = 0; z < TERRAIN_Z; z++) {
        
        fTerrainX = TERRAIN_SIZE/2;
        for (int x = 0; x < TERRAIN_X; x++) {
            
//            vertices[x + z * TERRAIN_X] = glm::vec3(fTerrainX, 0.0f, fTerrainZ);
            vertices[x + z * TERRAIN_X] = glm::vec3(fTerrainX, getHeight(x, z, height), fTerrainZ); // change height later
            /////////// Get min and max of vertex value for object centering in Window
            if (fTerrainX < minX) minX = fTerrainX;
            if (getHeight(x, z, height) < minY) minY = getHeight(x, z, height);
            if (fTerrainZ < minZ) minZ = fTerrainZ;
            
            if (fTerrainX > maxX) maxX = fTerrainX;
            if (getHeight(x, z, height) > maxY) maxY = getHeight(x, z, height);
            if (fTerrainZ > maxZ) maxZ = fTerrainZ;
            ///////////
            fTerrainX += TERRAIN_SIZE / (TERRAIN_X - 1);
        }
        
        fTerrainZ += TERRAIN_SIZE / (TERRAIN_Z - 1);
    }
//        for (glm::vec3 v: vertices) {
//            cout << "vertex: " << v.x << ", " << v.y << ", " << v.z << endl;
//        }
    
}

void Terrain::generateIndices() {
    
    for(int y = 0; y < TERRAIN_Z - 1; y++)
    {
        int base = y * TERRAIN_X;
        
        //indices[i++] = (uint16)base;
        for(int x = 0; x < TERRAIN_X; x++)
        {
            indices.push_back((GLuint)(base + x));
            indices.push_back((GLuint)(base + TERRAIN_X + x));
        }
        // add a degenerate triangle (except in a last row)
        if(y < TERRAIN_Z - 1)
        {
            indices.push_back((GLuint)((y + 1) * TERRAIN_X + (TERRAIN_X - 1)));
            indices.push_back((GLuint)((y + 1) * TERRAIN_X));
        }
    }
    
//    for (GLuint i: indices) {
//        cout << "index: " << i << endl;
//    }
    
//    for (int x = 0; x < TERRAIN_X - 1; ++x) {
//        for (int z = 0; z < TERRAIN_Z - 1; ++z) {
//            GLuint a = (x * TERRAIN_X) + z;
//            GLuint b = ((x+1) * TERRAIN_X) + z;
//            GLuint c = ((x+1) * TERRAIN_X) + (z+1);
//            GLuint d = (x * TERRAIN_X) + (z+1);
//
//            indices[numIndices++] = c;
//            indices[numIndices++] = b;
//            indices[numIndices++] = a;
//
//            indices[numIndices++] = a;
//            indices[numIndices++] = d;
//            indices[numIndices++] = c;
//        }
    
//    for (int x = 0; x < TERRAIN_X - 1; ++x) {
//        for (int z = 0; z < TERRAIN_Z - 1; ++z) {
//            indices.push_back((GLuint)((x * TERRAIN_X + z)));
//            indices.push_back((GLuint)(((x+1) * TERRAIN_X + z)));
//            indices.push_back((GLuint)((x * TERRAIN_X + (z+1))));
//
//            indices.push_back((GLuint)((x+1) * TERRAIN_X + z));
//            indices.push_back((GLuint)(x * TERRAIN_X + (z+1)));
//            indices.push_back((GLuint)((x+1) * TERRAIN_X + (z+1)));
            
//            indices.push_back((GLuint)((x + z * TERRAIN_X)));
//            indices.push_back((GLuint)(x + (z+1) * TERRAIN_X));
//            indices.push_back((GLuint)(((x+1) + z * TERRAIN_X)));
//
//            indices.push_back((GLuint)((x + (z+1) * TERRAIN_X)));
//            indices.push_back((GLuint)((x+1) + (z+1) * TERRAIN_X));
//            indices.push_back((GLuint)((x+1) + z * TERRAIN_X));
//        }
        
//    }
}

void Terrain::generateNormals() {
    for (int x = 1; x < TERRAIN_X; x++) {
        for (int z = 1; z < TERRAIN_Z; z++) { // or reverse z?

            float hl = getHeight(x, z, height);
            float hr = getHeight(x+1, z, height);
            float hd = getHeight(x, z+1, height); // terrain expands towards -z
            float hu = getHeight(x, z-1, height);

            glm::vec3 n = glm::vec3(hl - hr, 2.0f, hd - hu);
            
//            glm::vec3 hl = vertices[x + z * TERRAIN_X];
//            glm::vec3 hr = vertices[(x+1) + z * TERRAIN_X];
//            glm::vec3 hd = vertices[x + (z+1) * TERRAIN_X];
//            glm::vec3 hu = vertices[x + (z-1) * TERRAIN_X];
//
//            glm::vec3 n = glm::cross(hl - hr, hd - hu);
            
            n = glm::normalize(n);

            normals.push_back(n);
        }
    }
    
//    for (int x = 0; x < TERRAIN_X - 1; x++) {
//        for (int z = 0; z < TERRAIN_Z - 1; z++) {
//
////            glm::vec3 triangle0
////            = glm::vec3(getHeight(x, z, height), getHeight(x+1, z, height), getHeight(x+1, z+1, height));
////
////            glm::vec3 triangle1
////            = glm::vec3(getHeight(x+1, z+1, height), getHeight(x, z+1, height), getHeight(x, z, height));
//
//            glm::vec3 triangleNorm0
//            = glm::cross(vertices[x + z * TERRAIN_X] - vertices[(x+1) + z * TERRAIN_X],
//                         vertices[(x+1) + z * TERRAIN_X] - vertices[(x+1) + (z+1) * TERRAIN_X]);
//
//            glm::vec3 triangleNorm1
//            = glm::cross(vertices[(x+1) + (z+1) * TERRAIN_X] - vertices[x + (z+1) * TERRAIN_X],
//                         vertices[x + (z+1) * TERRAIN_X] - vertices[x + z * TERRAIN_X]);
//
//            normals.push_back(glm::normalize(triangleNorm0));
//            normals.push_back(glm::normalize(triangleNorm1));
//
//        }
//    }

}


void Terrain::draw(GLuint shaderProgram, glm::mat4 toWorld) {
    
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, rockTexture);
    
    glm::mat4 modelview = Window::V * toWorld; // before it was C
    // Now send these values to the shader program
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelview"), 1, GL_FALSE, &modelview[0][0]);
    //toWorld
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // see wireframe
    
    // Now draw the terrain. We simply need to bind the VAO associated with it.
    glBindVertexArray(VAO);
    // Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
//    glPointSize(2.0f);
//    glDrawArrays(GL_POINTS, 0, (GLsizei)vertices.size());
    glDrawElements(GL_TRIANGLE_STRIP, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
}
