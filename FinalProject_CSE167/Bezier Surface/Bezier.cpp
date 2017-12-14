#include "Bezier.h"
#include "Window.h"
#include <math.h>

using namespace std;

#define VERTEX_SHADER_PATH1 "shaderPlant.vert"
#define FRAGMENT_SHADER_PATH1 "shaderPlant.frag"

const char *flagImg = "flag_1.ppm";

//Factorial
int factorial(int n)
{
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

GLint shaderProgramCube;
Transform1::Transform1(glm::mat4 toW) {
	M = toW;
};
void Transform1::draw(glm::mat4 C) {

	for (auto child : children) {
		child->draw(C*M);
	}
};

void Transform1::update(glm::vec3 point[]) {

};
void Transform1::addChild(Node1* child) { children.push_back(child); };
void Transform1::removeChild(Node1* toD) {
	if ((std::find(children.begin(), children.end(), toD) != children.end())) {
		children.remove(toD);
	}
};
Geometry1::Geometry1(vector<glm::vec3> point, vector<unsigned> index,float rI,float gI,float bI){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    shaderProgramCube = LoadShaders(VERTEX_SHADER_PATH1, FRAGMENT_SHADER_PATH1);
    for(int i=0;i<point.size();i++){
        bazierCurve.push_back(point[i].x);
        bazierCurve.push_back(point[i].y);
        bazierCurve.push_back(point[i].z);
    }
    indices=index;
    r=rI;
    g=gI;
    b=bI;
    flag=0;
}

Geometry1::Geometry1(glm::vec3 point[],float rI,float gI,float bI) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
    r=rI;
    g=gI;
    b=bI;
    flag=1;
	for (int i = 0; i < 16; i++) {
		ctrlPoints[i] = point[i];
	}
    flagTexture = loadTexture(flagImg); //
	shaderProgramCube = LoadShaders(VERTEX_SHADER_PATH1, FRAGMENT_SHADER_PATH1);
    for (int i = 0; i <= 20; i ++) {
        for (int j =0; j<=20;j++){
            float k = float(i) / 20;
            float l = float(j) / 20;
            float x1 = 0, y1 = 0, z1 = 0;
            float con1 = 0;
            for (int m = 0; m < 4; m++) {
                con1 = (factorial(3) / (factorial(3 - m)*factorial(m)))*pow(k, m)*pow(1 - k, 3 - m);
                float con2=0;
                for (int n=0; n<4;n++){
                    con2 = (factorial(3) / (factorial(3 - n)*factorial(n)))*pow(l, n)*pow(1 - l, 3 - n);
                    x1 += con1*con2*ctrlPoints[m*4+n].x;
                    y1 += con1*con2*ctrlPoints[m*4+n].y;
                    z1 += con1*con2*ctrlPoints[m*4+n].z;
                }
            }
            bazierCurve.push_back(x1);
            bazierCurve.push_back(y1);
            bazierCurve.push_back(z1);
            
        }
    }
    for(int i=0;i<bazierCurve.size();i++){
//        cout<<bazierCurve[i]<<endl;
    }
    for (int i=0;i<20;i++){
        for(int j=0;j<20;j++){
            indices.push_back(i*21+j);
            indices.push_back(i*21+j+1);
            indices.push_back((i+1)*21+j+1);
            indices.push_back(i*21+j);
            indices.push_back((i+1)*21+j+1);
            indices.push_back((i+1)*21+j);
        }
    }
    
};

void Geometry1::draw(glm::mat4 C) {

	//float bazierCurve[6] = { ctrlPoints[0].x,ctrlPoints[0].y,ctrlPoints[0].z,ctrlPoints[3].x,ctrlPoints[3].y,ctrlPoints[3].z };
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*bazierCurve.size(), bazierCurve.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*indices.size(), &indices[0], GL_STATIC_DRAW);

	glUseProgram(shaderProgramCube);
	glm::mat4 modelview = C;
	auto uProjection = glGetUniformLocation(shaderProgramCube, "projection");
	auto uModelview = glGetUniformLocation(shaderProgramCube, "modelview");
	auto colorI = glGetUniformLocation(shaderProgramCube, "color");
    auto uToWorld = glGetUniformLocation(shaderProgramCube, "toWorld");
    auto uFlag = glGetUniformLocation(shaderProgramCube, "flag");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    auto toWorld=glm::mat4(1.0f);
    glUniformMatrix4fv(uToWorld, 1, GL_FALSE, &toWorld[0][0]);
    glUniform1i(uFlag, flag);
    glUniform3f(colorI, r, g, b);
	glBindVertexArray(VAO);
    
    // bind flag texture
    if (flag == 1) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, flagTexture);
        glUniform1i(glGetUniformLocation(shaderProgramCube, "tex_flag"), 1);
    }

	glDrawElements(GL_TRIANGLES, sizeof(unsigned)*2400, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
};

GLuint Geometry1::loadTexture(const char *textureFile) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load(textureFile, &width, &height, &nrChannels, 0);
    if (data) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    else cout << "Image failed to load at path: " << textureFile << endl;
    stbi_image_free(data);
    
//    glGenerateMipmap(GL_TEXTURE_2D);  // Generate mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    return textureID;
}

void Geometry1::update(glm::vec3 point[]) {
	for (int i = 0; i < 16; i++) {
		ctrlPoints[i] = point[i];
	}
    bazierCurve.clear();
    indices.clear();
    for (int i = 0; i <= 20; i ++) {
        for (int j =0; j<=20;j++){
            float k = float(i) / 20;
            float l = float(j) / 20;
            float x1 = 0, y1 = 0, z1 = 0;
            float con1 = 0;
            for (int m = 0; m < 4; m++) {
                con1 = (factorial(3) / (factorial(3 - m)*factorial(m)))*pow(k, m)*pow(1 - k, 3 - m);
                float con2=0;
                for (int n=0; n<4;n++){
                    con2 = (factorial(3) / (factorial(3 - n)*factorial(n)))*pow(l, n)*pow(1 - l, 3 - n);
                    x1 += con1*con2*ctrlPoints[m*4+n].x;
                    y1 += con1*con2*ctrlPoints[m*4+n].y;
                    z1 += con1*con2*ctrlPoints[m*4+n].z;
                }
            }
            bazierCurve.push_back(x1);
            bazierCurve.push_back(y1);
            bazierCurve.push_back(z1);
        }
    }
    
    for (int i=0;i<20;i++){
        for(int j=0;j<20;j++){
            indices.push_back(i*21+j);
            indices.push_back(i*21+j+1);
            indices.push_back((i+1)*21+j+1);
            indices.push_back(i*21+j);
            indices.push_back((i+1)*21+j+1);
            indices.push_back((i+1)*21+j);
        }
    }
};


