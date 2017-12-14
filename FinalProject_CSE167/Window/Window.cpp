#include "Window.h"
#include <math.h>
#include "shader.h"
#include "light.h"
#include <time.h>

float zVal = 0.0f;
const char* window_title = "My Island";
const char* body_obj = "body.obj";
const char* cube_obj = "cube.obj";
const char* roof_obj = "roof.obj";

GLint Window::shaderProgram_skybox;
GLint Window::shaderProgram_terrain;
GLint Window::shaderProgram_plant;
GLint Window::shaderProgram_house;
GLint Window::shaderProgram_obj; // bunny debugging

vector<glm::vec3> terrainVert;
vector<glm::vec3> treeVert;
int plantMove=0;


// On some systems you need to change this to the absolute path
#define SKYBOX_VERTEX_SHADER_PATH "skyboxShader.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "skyboxShader.frag"
#define TERRAIN_VERTEX_SHADER_PATH "terrainShader.vert"
#define TERRAIN_FRAGMENT_SHADER_PATH "terrainShader.frag"

#define PLANT_VERTEX_SHADER_PATH "shaderPlant.vert"
#define PLANT_FRAGMENT_SHADER_PATH "shaderPlant.frag"

#define HOUSE_VERTEX_SHADER_PATH "houseshader.vert"
#define HOUSE_FRAGMENT_SHADER_PATH "houseshader.frag"

// bunny debugging
#define OBJ_VERTEX_SHADER_PATH "objshader.vert"
#define OBJ_FRAGMENT_SHADER_PATH "objshader.frag"

// Default camera parameters
////glm::vec3 Window::cam_pos = glm::vec3(-400.0f, 200.0f, 30.0f);        // e  | Position of camera
//glm::vec3 Window::cam_pos(-100.0f, 50.0f, 10.0f);        // e  | Position of camera

glm::vec3 Window::cam_pos(0.0f, 0.0f, 500.0f);        // e  | Position of camera
//glm::vec3 Window::cam_pos(0.0f, 0.0f, 5.0f);        // e  | Position of camera

glm::vec3 Window::cam_look_at = glm::vec3(0.0f, 0.0f, 0.0f);    // d  | This is where the camera looks at
glm::vec3 Window::cam_up = glm::vec3(0.0f, 1.0f, 0.0f);            // up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

double Window::xpos;
double Window::ypos;

double Window::OLD_XPOS;
double Window::OLD_YPOS;
double Window::scrollY = 0.0f;

glm::vec3 Window::A;
glm::vec3 Window::B;
float Window::angle;
glm::vec3 Window::axis;

bool Window::CAMERA_ROTATE;
bool Window::TERRAIN_ROTATE;
bool Window::TERRAIN_UP;
bool Window::TERRAIN_DOWN;
bool Window::TERRAIN_LEFT;
bool Window::TERRAIN_RIGHT;
bool Window::TERRAIN_SWITCH;

bool CAM_MOVE;

glm::mat4 Window::terrainMat = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f)); // change to 2.0f
glm::mat4 skyboxMat = glm::scale(glm::mat4(1.0f), glm::vec3(6.0f));
Skybox * skybox;
Terrain * terrain;
Plant * plant1,* plant2,* plant3,* plant4,* plant5,* plant6,* plant7,* plant8;

Transform * island;
float midX, midY, midZ;

/*   Bezier    */
Transform1 * flag;
Transform1 * stickTrans;
Geometry1 * stick;
Geometry1 * bezierPach;
float up=0.4;
int countUp=0;
glm::vec3 bezierPoints[16]={
    glm::vec3(0.0f,30.0f,0.0f),
    glm::vec3(15.0f,20.0f,0.0f),
    glm::vec3(30.0f,30.0f,0.0f),
    glm::vec3(45.0f,30.0f,0.0f),
    glm::vec3(0.0f,20.0f,0.0f),
    glm::vec3(15.0f,20.0f,0.0f),
    glm::vec3(30.0f,10.0f,0.0f),
    glm::vec3(45.0f,20.0f,0.0f),
    glm::vec3(0.0f,10.0f,0.0f),
    glm::vec3(15.0f,00.0f,0.0f),
    glm::vec3(30.0f,10.0f,0.0f),
    glm::vec3(45.0f,10.0f,0.0f),
    glm::vec3(0.0f,0.0f,0.0f),
    glm::vec3(15.0f,-10.0f,0.0f),
    glm::vec3(30.0f,0.0f,0.0f),
    glm::vec3(45.0f,0.0f,0.0f),
};

/* House */
Transform* boat;
Transform* map;
Transform* buildMap;
Transform* house;
Transform* roofmtx;
Transform* bodymtx;
Geometry* body;
Geometry* roof;

/* Light */
light* light_ptr;
light dir_light(0);

// Bunny debugging
OBJObject * obj;

//const char *noise1 = "noise1.ppm";
//const char *noise2 = "noise2.ppm";
//char* noiseFile;

void Window::initialize_objects()
{
    // Bunny debugging
    shaderProgram_obj = LoadShaders(OBJ_VERTEX_SHADER_PATH, OBJ_FRAGMENT_SHADER_PATH);
    obj = new OBJObject("capsule.obj");
    
    /* Bazier */
    plant1=new Plant();
    plant2=new Plant();
    plant3=new Plant();
    plant4=new Plant();
    plant5=new Plant();
    plant6=new Plant();
    plant7=new Plant();
    plant8=new Plant();
    
    
    flag=new Transform1(glm::translate(glm::mat4(1.0f), glm::vec3(30.0f,70.0f,0.0f))*glm::scale(glm::mat4(1.0f),glm::vec3(2.0f,2.0f,2.0f)));
    bezierPach = new Geometry1(bezierPoints,0.1f,0.1f,0.8f);
    stick= new Geometry1(plant1->vertices,plant1->indices,0.5f,0.5f,0.5f);
    stickTrans=new Transform1(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,45.0f,0.0f))*glm::scale(glm::mat4(1.0f), glm::vec3(1.0f,4.0f,1.0f))*glm::rotate(glm::mat4(1.0f),90/ 180.0f * glm::pi<float>(), glm::vec3(1.5f, 0.0f, 1.0f)));
    stickTrans->addChild(stick);
    flag->addChild(stickTrans);
    flag->addChild(bezierPach);
    
    skybox = new Skybox();
    skybox->toWorld = skyboxMat;
    terrain = new Terrain();
    terrainVert = Terrain::vert;
    
    island = new Transform(glm::mat4(1.0f));
    island->addChild(terrain);
	// Load the shader program. Make sure you have the correct filepath up top
    shaderProgram_skybox = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);
    shaderProgram_terrain = LoadShaders(TERRAIN_VERTEX_SHADER_PATH, TERRAIN_FRAGMENT_SHADER_PATH);
    shaderProgram_plant = LoadShaders(PLANT_VERTEX_SHADER_PATH, PLANT_FRAGMENT_SHADER_PATH);
    shaderProgram_house = LoadShaders(HOUSE_VERTEX_SHADER_PATH, HOUSE_FRAGMENT_SHADER_PATH);
    
    midX = (Terrain::maxX + Terrain::minX)/2.0f;
    midY = (Terrain::maxY + Terrain::minY)/2.0f;
    midZ = (Terrain::maxZ + Terrain::minZ)/2.0f;
    
    terrainMat = terrainMat * glm::translate(glm::mat4(1.0f), glm::vec3((-1.0f)*midX, (-1.0f)*midY, (-1.0f)*midZ));
    
    /* House */
    buildMap = new Transform(glm::mat4(1.0f));
    srand((unsigned int)time(NULL));
    
    for (int j = 0; j < 6; j++) {
        roof = new Geometry(roof_obj);
        roof->rotate(-3.14 / 2, glm::vec3(1, 0, 0));
        roof->rotate(3.14 / 2, glm::vec3(0, 1, 0));
        roof->diffuse = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
        roof->ambient = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
        roof->specular = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
        roof->shininess = rand() % 100;
        int floorNum = rand()%14+3;
//        cout << floorNum << endl;
        Transform* fl = new Transform(glm::mat4(1.0f));

        double lastSize = 0;
        for (int i = 0; i < floorNum; i++) {
            Geometry* cube = new Geometry(cube_obj);
            cube->diffuse = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
            cube->ambient = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
            cube->specular = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
            cube->shininess = rand() % 100;
            Transform* cubemtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, -7+i, 0)));
            lastSize = 2 * (double)rand() / RAND_MAX + 4;
            cubemtx->scale(lastSize, 0.5, lastSize);
            cubemtx->addChild(cube);
            fl->addChild(cubemtx);
        }
        house = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(100 * j - 200, -10, 120)));
        house->scale(3,3,3);
        if (floorNum > 7) {
            body = new Geometry(body_obj);
            body->diffuse = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
            body->ambient = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
            body->specular = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
            body->shininess = rand() % 100;
            bodymtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(lastSize*2-2, -2.5, lastSize * 0.5-2)));
            bodymtx->scale(floorNum, 5, floorNum-1);
            bodymtx->addChild(body);
            house->addChild(bodymtx);
        }

        roofmtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, -7 + floorNum, 0)));
        roofmtx->scale(lastSize+1, lastSize+1, lastSize+1);
        roofmtx->addChild(roof);

        house->addChild(roofmtx);
        house->addChild(fl);
        buildMap->addChild(house);
    }
}

void Window::display_callback(GLFWwindow* window)
{
    
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the shader of programID
    glUseProgram(shaderProgram_skybox);

    // Render the object
    skybox->draw(shaderProgram_skybox);
    
    // Bunny test
//        obj->toWorld = terrainMat;
//    glUseProgram(shaderProgram_obj);
//    obj->draw(shaderProgram_obj);
    
    glm::mat4 transform = glm::mat4(1.0f);
    if (TERRAIN_UP) {
        transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 15.0f, 0.0f));
        terrainMat = transform * terrainMat;
        TERRAIN_UP = false;
    }
    if (TERRAIN_DOWN) {
        transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -15.0f, 0.0f));
        terrainMat = transform * terrainMat;
        TERRAIN_DOWN = false;
    }
    if (TERRAIN_LEFT) {
        transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -15.0f));
        terrainMat = transform * terrainMat;
        TERRAIN_LEFT = false;
    }
    if (TERRAIN_RIGHT) {
        transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 15.0f));
        terrainMat = transform * terrainMat;
        TERRAIN_RIGHT = false;
    }
    
    // Draw terrain
    glUseProgram(shaderProgram_terrain);
    terrain->draw(shaderProgram_terrain, terrainMat);
//    island->draw(shaderProgram_terrain, terrainMat);
    
    // Draw plants
    glUseProgram(shaderProgram_plant);
    for (int i=0; i<treeVert.size();i++){
        
    }
    string rule1[2]={"AABBAA", "BBA"};
    string rule2[2]={"AABBAAB", "BABA"};
    string rule3[2]={"AABA", "AABBA"};
    string rule4[2]={"AAB", "BBABB"};
    plant1->draw(shaderProgram_plant, 3, glm::mat4(1.0f), glm::vec3(0.0f,-10.f,20.0f), 'A', rule4);
    plant2->draw(shaderProgram_plant, 3, glm::mat4(1.0f), glm::vec3(50.0f,-40.f,200.0f), 'A', rule3);
    plant3->draw(shaderProgram_plant, 3, glm::mat4(1.0f), glm::vec3(-100.0f,-10.f,20.0f), 'A', rule2);
    plant4->draw(shaderProgram_plant, 3, glm::mat4(1.0f), glm::vec3(200.0f,-40.f,150.0f), 'A', rule2);
    plant5->draw(shaderProgram_plant, 3, glm::mat4(1.0f), glm::vec3(100.0f,-30.f,100.0f), 'A', rule1);
    plant6->draw(shaderProgram_plant, 3, glm::mat4(1.0f), glm::vec3(10.0f,-10.f,70.0f), 'A', rule2);
    plant7->draw(shaderProgram_plant, 3, glm::mat4(1.0f), glm::vec3(80.0f,-10.f,-30.0f), 'A', rule3);
    plant8->draw(shaderProgram_plant, 3, glm::mat4(1.0f), glm::vec3(-100.0f,-10.f,80.0f), 'A', rule1);
    flag->draw(V);
    
    // Draw houses
    glUseProgram(shaderProgram_house);
    buildMap->draw(shaderProgram_house,glm::mat4(1.0f));
    
    light_ptr = &dir_light;
    light_ptr->draw(shaderProgram_house);
    
    // Gets events, including input such as keyboard and mouse or window resizing
    glfwPollEvents();
    // Swap buffers
    glfwSwapBuffers(window);

}

void Window::idle_callback()
{
    skybox->toWorld = glm::translate(glm::mat4(1.0f), cam_pos);
//    if (CAM_MOVE) {
//        terrainMat = glm::translate(glm::mat4(1.0f), cam_pos) * terrainMat;
//        CAM_MOVE = false;
//    }
    
    /* Bezier*/
    countUp+=1;
    // Call the update function the cube
    for (int i=15;i>=0;i--){
        if(countUp==25){
            countUp=0;
            up=(up==0.4f)?-0.4f:0.4f;
        }
        if (i%4 >=2){
            bezierPoints[i].y=bezierPoints[i-1].y;
            bezierPoints[i].z=bezierPoints[i-1].z;
        }
        else if (i%4==1){
            bezierPoints[i].y+=up;
            bezierPoints[i].z+=up;
        }
    }
    
    bezierPach->update(bezierPoints);
    
    /* Plant move*/
    if(plantMove>0){
        plant1->update(1);
        plant2->update(1);
        plant3->update(1);
        plant4->update(1);
        plant5->update(1);
        plant6->update(1);
        plant7->update(1);
        plant8->update(1);
        plantMove+=1;
    }
    else{
        plant1->update(-1);
        plant2->update(-1);
        plant3->update(-1);
        plant4->update(-1);
        plant5->update(-1);
        plant6->update(-1);
        plant7->update(-1);
        plant8->update(-1);
        plantMove+=1;
    }
    if(plantMove==10){
        plantMove=-9;
    }
    
}

void Window::update() {
    
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
    delete(skybox);
    glDeleteProgram(shaderProgram_skybox);
    // more
    glDeleteProgram(shaderProgram_terrain);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);
	// Set swap interval to 1
	glfwSwapInterval(1);
	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);
	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
//        P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
        P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 3000.0f);
        V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
        }
        switch(key) {
            case GLFW_KEY_UP:
                TERRAIN_UP = true;
                break;
                
            case GLFW_KEY_DOWN:
                TERRAIN_DOWN = true;
                break;
                
            case GLFW_KEY_LEFT:
                TERRAIN_LEFT = true;
                break;
                
            case GLFW_KEY_RIGHT:
                TERRAIN_RIGHT = true;
                break;
                
            case GLFW_KEY_T:
                TERRAIN_SWITCH = true;
                break;
        }
        if (key == GLFW_KEY_R) {
            /* House */
            buildMap = new Transform(glm::mat4(1.0f));
            srand((unsigned int)time(NULL));
            
            for (int j = 0; j < 6; j++) {
                roof = new Geometry(roof_obj);
                roof->rotate(-3.14 / 2, glm::vec3(1, 0, 0));
                roof->rotate(3.14 / 2, glm::vec3(0, 1, 0));
                roof->diffuse = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
                roof->ambient = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
                roof->specular = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
                roof->shininess = rand() % 100;
                int floorNum = rand()%14+3;
                //        cout << floorNum << endl;
                Transform* fl = new Transform(glm::mat4(1.0f));
                
                double lastSize = 0;
                for (int i = 0; i < floorNum; i++) {
                    Geometry* cube = new Geometry(cube_obj);
                    cube->diffuse = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
                    cube->ambient = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
                    cube->specular = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
                    cube->shininess = rand() % 100;
                    Transform* cubemtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, -7+i, 0)));
                    lastSize = 2 * (double)rand() / RAND_MAX + 4;
                    cubemtx->scale(lastSize, 0.5, lastSize);
                    cubemtx->addChild(cube);
                    fl->addChild(cubemtx);
                }
                house = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(100 * j - 200, -10, 120)));
                house->scale(3,3,3);
                if (floorNum > 7) {
                    body = new Geometry(body_obj);
                    body->diffuse = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
                    body->ambient = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
                    body->specular = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
                    body->shininess = rand() % 100;
                    bodymtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(lastSize*2-2, -2.5, lastSize * 0.5-2)));
                    bodymtx->scale(floorNum, 5, floorNum-1);
                    bodymtx->addChild(body);
                    house->addChild(bodymtx);
                }
                
                roofmtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, -7 + floorNum, 0)));
                roofmtx->scale(lastSize+1, lastSize+1, lastSize+1);
                roofmtx->addChild(roof);
                
                house->addChild(roofmtx);
                house->addChild(fl);
                buildMap->addChild(house);
            }
            
        }
	}
    if (action == GLFW_RELEASE) {
//        TERRAIN_UP = false;
    }
}

// Map 2D cursor position to 3D; do not change this method
glm::vec3 Window::trackBallMapping(GLFWwindow* window, double xpos, double ypos) {
    glm::vec3 v;
    float d;
    int w = Window::width;
    int h = Window::height;
    
    v.x = (2.0 * xpos - w)/w;
    v.y = (h - 2.0 * ypos)/h;
    v.z = 0.0;
    d = glm::length(v);
    d = (d < 1) ? d : 1.0; // if d is smaller than 1, set d to 1
    v.z = sqrtf(1.0001 - d*d); // or pow(d, 2.0)
    v = glm::normalize(v);
    return v;
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    
    if (action == GLFW_PRESS) {
        
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT: 
                CAMERA_ROTATE = true;
                break;
                
            case GLFW_MOUSE_BUTTON_RIGHT:
//                TERRAIN_ROTATE = true;
                break;
                
            default:
                break;
        }
    }
    if (action == GLFW_RELEASE) {
        CAMERA_ROTATE = false;
        TERRAIN_ROTATE = false;
    }
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

    A = trackBallMapping(window, OLD_XPOS, OLD_YPOS);
    B = trackBallMapping(window, xpos, ypos);

    if (A == B) {
        return;
    }
    
    float velocity = length(B-A);
    if (velocity < 0.0001) return;
    
    angle = velocity;
    axis = glm::cross(A, B);

    if (CAMERA_ROTATE) {
        glm::mat3 R = glm::mat3(glm::rotate(glm::mat4(1.0f), angle*50/180 * glm::pi<float>(), axis));
        cam_pos = R * cam_pos;
        V = glm::lookAt(cam_pos, cam_look_at, cam_up);
    }
    
    if (TERRAIN_ROTATE) {
        if (axis.y > 0) {
            terrainMat = terrainMat * glm::rotate(glm::mat4(1.0f), angle*70/180 * glm::pi<float>(), glm::vec3(0.0f, -1.0f, 0.0f));
        } else {
            terrainMat = terrainMat * glm::rotate(glm::mat4(1.0f), angle*70/180 * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
        }
    }
    
    OLD_XPOS = xpos;
    OLD_YPOS = ypos;
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    scrollY += yoffset;
    if (yoffset > 0) {
        cam_pos = 0.95f * cam_pos;
//        CAM_MOVE = true;
    }
    else if (yoffset < 0) {
        cam_pos = 1.05f * cam_pos;
//        CAM_MOVE = true;
    }
    V = glm::lookAt(cam_pos, cam_look_at, cam_up);
    scrollY = 0.0;
}


