//
//  Terrain.cpp
//  FinalProject_CSE167
//
//  Created by Brittany Zhang on 12/5/17.
//  Copyright © 2017 ManianaBonita. All rights reserved.
//

#include "Terrain.h"
#include "Window.h"

#define RAW_WIDTH 100 // to create a grid of 256 by 256 square patches
#define RAW_HEIGHT 100 // with each patch made up of two triangles
// each of which requires 3 triangles to be rendered

#define HEIGHTMAP_X 16.0f
#define HEIGHTMAP_Z 16.0f
#define HEIGHTMAP_Y 1.25f
// tex_x and tex_z


const int SCALE = 90.0f;
//const int cols = Window::width / SCALE;
//const int rows = Window::height / SCALE;

const char *noise1 = "noise1.ppm";
const char *noise2 = "noise2.ppm";
const char *rockImg = "rock.ppm";

float Terrain::getHeight(int x, int z, int height) {
    if ((float)x + (float)z * (float)height > yVal.size()-1) return 0.0f;
    return yVal[(float)x + (float)z * (float)height] * SCALE;
}

Terrain::Terrain() {

    int width, height, nrChannels;
    unsigned char *rgb = stbi_load(noise1, &width, &height, &nrChannels, 0); // height map
    if (!rgb) cout << "Image failed to load at path: " << noise1 << endl;
    // rgb is now three bytes per pixel, width * height size. Or NULL if load failed.
    
    for (size_t i = 0; i < width * height * 3; i += 3) {
        yVal.push_back((float)rgb[i]/255.0f);
    }
    stbi_image_free(rgb);
    
    


    generateHeightMap(width, height);
//    generateVertices();
    generateVertices(width, height);
//    generateIndices(vertices, width, height);
    generateIndices();
//    generateNormals(vertices, width, height);

    
    ///////////////////////////////////
    
//    loadTexture(rockImg, rockTexture);
    
    // Binding data
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat) * 3, vertices.data(), GL_STATIC_DRAW); // &vertices[0].x
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
//    glBindBuffer(GL_ARRAY_BUFFER, NBO);
//    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat) * 3, normals.data(), GL_STATIC_DRAW); // sizeof(glm::vec3)
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &(indices[0]), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
//        for (glm::vec3 v: vertices) {
//            cout << "vertices: " << v.x << ", " << v.y << ", " << v.z << endl;
//        }
    
//        for (GLuint i: indices) {
//            cout << "indice: " << i << endl;
//        }
    
//        for (glm::vec3 n: normals) {
//            cout << "normal: " << n.x << ", " << n.y << ", " << n.z << endl;
//        }
    
}

void Terrain::generateVertices() {
    for (int x = 0; x < RAW_WIDTH*RAW_HEIGHT; x++) {
        vertices.push_back(glm::vec3(1.0f));
    }
    
    for (int x = 0; x < RAW_WIDTH; ++x) {
        for (int z = 0; z < RAW_HEIGHT; ++z) {
            int offset = (x * RAW_WIDTH) + z;
            
            vertices[offset] = glm::vec3(x * HEIGHTMAP_X, yVal[offset] * HEIGHTMAP_Y * SCALE, z * HEIGHTMAP_Z);
            
            // texture
        }
    }
}

void Terrain::generateVertices(unsigned int width, unsigned int height) {
    //    int i = 0;
    for (int x = 0; x < width; x++) {
        for (int z = 0; z < height; z++) {
            //            vertices.push_back(glm::vec3((float)x, yVal[i]*SCALE, (float)z));
            //            i++;
            
            glm::vec3 A = glm::vec3(x, getHeight(x, z, height), z);
            glm::vec3 B = glm::vec3(x+1, getHeight(x+1, z, height), z);
            glm::vec3 C = glm::vec3(x+1, getHeight(x+1, z+1, height), z+1);
            glm::vec3 D = glm::vec3(x, getHeight(x, z+1, height), z+1);
            
            vertices.push_back(A);
            vertices.push_back(B);
            vertices.push_back(C);
            vertices.push_back(A);
            vertices.push_back(C);
            vertices.push_back(D);
            
            //            normals.push_back(glm::normalize(glm::cross(B-A, D-A))); // A
            //            normals.push_back(glm::normalize(glm::cross(A-B, C-B))); // B
            //            normals.push_back(glm::normalize(glm::cross(B-C, D-C))); // C
            //            normals.push_back(glm::normalize(glm::cross(B-A, D-A))); // A
            //            normals.push_back(glm::normalize(glm::cross(B-C, D-C))); // C
            //            normals.push_back(glm::normalize(glm::cross(A-D, C-D))); // D
        }
    }
}

void Terrain::generateIndices() {
    for (int x = 0; x < (RAW_WIDTH-1) * (RAW_HEIGHT-1) * 6; x++) {
        indices.push_back((GLuint)0);
    }
    
    int numIndices = 0;
    for (int x = 0; x < RAW_WIDTH - 1; ++x) {
        for (int z = 0; z < RAW_HEIGHT - 1; ++z) {
            GLuint a = (x * RAW_WIDTH) + z;
            GLuint b = ((x+1) * RAW_WIDTH) + z;
            GLuint c = ((x+1) * RAW_WIDTH) + (z+1);
            GLuint d = (x * RAW_WIDTH) + (z+1);
            
            indices[numIndices++] = c;
            indices[numIndices++] = b;
            indices[numIndices++] = a;
            
            indices[numIndices++] = a;
            indices[numIndices++] = d;
            indices[numIndices++] = c;
        }
    }
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

void Terrain::generateIndices(vector<glm::vec3> vertices, int width, int height) {
    // populate indices
    for(int y = 0; y < height - 1; y++)
    {
        int base = y * width;
        
        //indices[i++] = (uint16)base;
        for(int x = 0; x < width; x++)
        {
            indices.push_back((GLuint)(base + x));
            indices.push_back((GLuint)(base + width + x));
        }
        // add a degenerate triangle (except in a last row)
        if(y < height - 1)
        {
            indices.push_back((GLuint)((y + 1) * width + (width - 1)));
            indices.push_back((GLuint)((y + 1) * width));
        }
    }
}

void Terrain::generateNormals(vector<glm::vec3> vertices, unsigned int width, unsigned int height) {

    for (int x = 1; x < width; x++) {
        for (int z = 1; z < height; z++) {
            
            float hl = getHeight(x, z, height);
            float hr = getHeight(x+1, z, height);
            float hd = getHeight(x, z+1, height); // terrain expands towards -z
            float hu = getHeight(x, z-1, height);
            
            glm::vec3 n = glm::vec3(hl - hr, 2.0f, hd - hu);
            n = glm::normalize(n);
            
            normals.push_back(n);
        }
    }

    // populate normals
    //    triangle ( v1, v2, v3 )
    //    edge1 = v2-v1
    //    edge2 = v3-v1
    //    triangle.normal = cross(edge1, edge2).normalize()
    
    //    for (int k = 0; k < vertices.size()-2; k += 2) {
    //        glm::vec3 edge1 = vertices[k+1] - vertices[k];
    //        glm::vec3 edge2 = vertices[k+2] - vertices[k];
    //        float nx = (edge1.y * edge2.z) - (edge1.z * edge2.y);
    //        float ny = (edge1.z * edge2.x) - (edge1.x * edge2.z);
    //        float nz = (edge1.x * edge2.y) - (edge1.y * edge2.x);
    //        normals.push_back(glm::vec3(nx, ny, nz));
    
    //        glm::vec3 n = glm::normalize(glm::cross(edge1, edge2));
    //        normals.push_back(n);
    
    //        cout << "normal: " << n.x << ", " << n.y << ", " << n.z << endl;
    //    }
    
}

void Terrain::loadTexture(const char *textureFile, GLuint textureID) {
    // bind textures
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // load texture image
    int width, height, nrChannels;
    unsigned char *data = stbi_load(textureFile, &width, &height, &nrChannels, 0);
    if (!data) cout << "Image failed to load at path: " << rockImg << endl;
    
    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    stbi_image_free(data);
    
    //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Terrain::~Terrain() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &NBO);
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

    // Now draw the terrain. We simply need to bind the VAO associated with it.
    glBindVertexArray(VAO);
    // Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
    glPointSize(2.0f);
    glDrawArrays(GL_POINTS, 0, (GLsizei)vertices.size());
//    glDrawElements(GL_TRIANGLE_STRIP, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
}
