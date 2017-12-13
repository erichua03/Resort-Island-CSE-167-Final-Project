#include "Plant.h"
#include "Window.h"
#include <math.h>
#include <stdlib.h>     /* srand, rand */
using namespace std;

Plant::Plant()
{
    toWorld = glm::mat4(1.0f);
    parse("antenna.obj");
    angleA=40;
    angleB=20;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
    // Consider the VAO as a container for all your buffers.
    glBindVertexArray(VAO);
    
    // Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
    // you want to draw, such as vertices, normals, colors, etc.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
    // the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
    // Enable the usage of layout location 0 (check the vertex shader to see what this is)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    
    // We've sent the vertex data over to OpenGL, but there's still something missing.
    // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*indices.size(), &indices[0], GL_STATIC_DRAW);
    
    // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO now so we don't accidentally tamper with it.
    // NOTE: You must NEVER unbind the element array buffer associated with a VAO!
    glBindVertexArray(0);
    
}

Plant::~Plant()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Plant::parse(const char *filepath) {
    // Populate the face indices, vertices, and normals vectors with the OBJ Object data
    FILE* fp;     // file pointer
    float x, y, z;  // vertex coordinates
    float r, g, b;  // vertex color
    int c1, c2;    // characters read from file
    errno = 0;
    unsigned a, b2, c, d, e, f;
    
    fp = fopen(filepath, "rb");  // make the file name configurable so you can load other files
    if (fp == NULL) { cerr << "error loading file" << endl; cerr << errno << endl; exit(-1); }  // just in case the file can't be found or is corrupt
    
    while (!feof(fp)) {
        c1 = fgetc(fp);
        c2 = fgetc(fp);
        
        if (c1 == 'v'&&c2 == 'n') {
            fscanf(fp, "%f %f %f", &x, &y, &z);
            //glm::vec3 temp=new glm::vec3(x,y,z);
            auto temp = glm::normalize(glm::vec3(x, y, z));
            //cout<<to_string(glm::vec3(x,y,z))<<":"<<to_string(temp)<<endl;
            normals.push_back(glm::vec3((temp.x + 1) / 2, (temp.y + 1) / 2, (temp.z + 1) / 2));
        }
        else if (c1 == 'v' && c2 == ' ') {
            fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
            //glm::vec3 temp=new glm::vec3(x,y,z);
            vertices.push_back(glm::vec3(x, y, z));
        }
        else if (c1 == '#') {
            fscanf(fp, "%*[^\n]\n", NULL);
        }
        else if (c1 == 'f'&& c2 == ' ') {
            fscanf(fp, "%u//%u %u//%u %u//%u.\n", &a, &b2, &c, &d, &e, &f);
            indices.push_back(a - 1);
            indices.push_back(c - 1);
            indices.push_back(e - 1);
        }
    }
    fclose(fp);   // make sure you don't forget to close the file when done
}

void Plant::draw(GLuint shaderProgram,int n, glm::mat4 trans, glm::vec3 start, char input,std::string rules[])
{
    //cout<<n<<endl;
    // Calculate the combination of the model and view (camera inverse) matrices
    glm::mat4 trans1=trans*glm::rotate(glm::mat4(1.0f),-90/ 180.0f * glm::pi<float>(), glm::vec3(1.5f, 0.0f, 1.0f));
    glm::mat4 modelview = Window::V * toWorld;
    // We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
    // Consequently, we need to forward the projection, view, and model matrices to the shader programs
    // Get the location of the uniform variables "projection" and "modelview"
    uProjection = glGetUniformLocation(shaderProgram, "projection");
    uModelview = glGetUniformLocation(shaderProgram, "modelview");
    auto uToWorld = glGetUniformLocation(shaderProgram, "toWorld");
    auto uColor = glGetUniformLocation(shaderProgram, "color");
    auto uFlag = glGetUniformLocation(shaderProgram, "flag");
    // Now send these values to the shader program
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    glUniformMatrix4fv(uToWorld, 1, GL_FALSE, &(glm::translate(glm::mat4(1.0f),start)*trans1)[0][0]);
    glUniform1i(uFlag, 0);
    
//    if(n>Window::layers-2){
    if(n > 3){
        glUniform3f(uColor, 0.8f, 0.4f, 0.1f);
    }
    else{
        glUniform3f(uColor, 1/float(n+1.5f), 0.7f, 0.0f);
    }
    
    // Now draw the cube. We simply need to bind the VAO associated with it.
    glBindVertexArray(VAO);
    // Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
    glDrawElements(GL_TRIANGLES, sizeof(unsigned)*indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
    string next;
    if (input=='A')
        next=rules[0];
    else
        next=rules[1];
    if (n>=1){
        for (int i=0; i<next.length();i++){
            if (next[i]=='A'){
                auto transM=glm::rotate(glm::mat4(1.0f),i*(360.0f/next.length())/ 180.0f * glm::pi<float>(), glm::vec3(trans*glm::vec4(0.0f, 1.0f, 0.0f,0.0f)))
                    *glm::rotate(glm::mat4(1.0f),angleA/ 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f))
                    *glm::scale(glm::mat4(1.0f), glm::vec3(0.7f,0.7f,0.7f))*trans;
                draw(shaderProgram,n-1,transM,glm::vec3((glm::translate(glm::mat4(1.0f),start))*trans1*glm::vec4(vertices[2282][0],vertices[2282][1],vertices[2282][2],1.0f)),next[i],rules);
            }
            else if (next[i]=='B'){
                auto transM=glm::rotate(glm::mat4(1.0f),i*(360.0f/next.length())/ 180.0f * glm::pi<float>(), glm::vec3(trans*glm::vec4(0.0f, 1.0f, 0.0f,0.0f)))
                *glm::rotate(glm::mat4(1.0f),angleB/ 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f))
                *glm::scale(glm::mat4(1.0f), glm::vec3(0.9f,0.9f,0.9f))*trans;
                draw(shaderProgram,n-1,transM,glm::vec3((glm::translate(glm::mat4(1.0f),start))*trans1*glm::vec4(vertices[2282][0],vertices[2282][1],vertices[2282][2],1.0f)),next[i],rules);
            }
        }
    }
}

void Plant::update(int i){
    if (i==1){
        if (angleA<90){
            angleA+=10;
            angleB+=10;
        }
    }
    else{
        if (angleA>0){
            angleA-=10;
            angleB-=10;
        }
    }
}
