#include "window.h"
#include "Transform.h"
#include "Geometry.h"
#include "Node.h"
#include "Bezier.h"
#include "light.h"
#include <time.h>  

using namespace std;
const char* window_title = "GLFW Starter Project";
bool up = false;
GLint shaderProgram;
GLint skyboxShaderProgram;
GLint mapShaderProgram;
// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"

light* light_ptr;
light dir_light(0);

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

int mouse;

glm::mat4 Window::P;
glm::mat4 Window::V;

glm::vec2 Window::prePos;
glm::vec2 Window::currPos;
glm::vec2 Window::cursor_pos;

Skybox* skybox;

#define SKYBOX_VERTEX_SHADER_PATH "../skybox.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "../skybox.frag"

#define MAP_VERTEX_SHADER_PATH "../Environment.vert"
#define MAP_FRAGMENT_SHADER_PATH "../Environment.frag"

Bezier * curve;
Geometry * sphere;
Transform* car;
Transform* map;
Transform* buildMap;
Transform* house;
Transform* roofmtx;
Transform* bodymtx;
static bool stop;
int selectID;
int currLocation = 0;
float scale = 10.0f;
double prex, prey;
static bool rider;

Geometry* body;

Geometry* roof;
void Window::initialize_objects()
{
	

	sphere = new Geometry("../boat.obj");
	car = new Transform(glm::mat4(1.0f));
	car->rotate(3.14/2,glm::vec3(0,1,0));
	car->rotate(3.14/10, glm::vec3(1, 0, 0));
	car->translate(0, -5, -10);
	car->addChild(sphere);
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skyboxShaderProgram = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);
	mapShaderProgram = LoadShaders(MAP_VERTEX_SHADER_PATH, MAP_FRAGMENT_SHADER_PATH);
	// Load the shader program. Make sure you have the correct filepath up top
	skybox = new Skybox();
	
	//body = new Geometry("../building/body.obj");

	

	buildMap = new Transform(glm::mat4(1.0f));
	srand(time(NULL));

	for (int j = 0; j < 3; j++) {
		roof = new Geometry("../building/roof.obj");
		roof->rotate(-3.14 / 2, glm::vec3(1, 0, 0));
		roof->rotate(3.14 / 2, glm::vec3(0, 1, 0));
		roof->diffuse = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
		roof->ambient = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
		roof->specular = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
		roof->shininess = rand() % 100;
		int floorNum = rand()%14+3;
		cout << floorNum << endl;
		Transform* fl = new Transform(glm::mat4(1.0f));

		double lastSize = 0;
		for (int i = 0; i < floorNum; i++) {
			Geometry* cube = new Geometry("../building/cube.obj");
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
		house = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(30 * j - 30, 0, -50)));
		if (floorNum > 7) {
			body = new Geometry("../building/body.obj");
			body->diffuse = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
			body->ambient = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
			body->specular = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),  static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
			body->shininess = rand() % 100;
			bodymtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(lastSize*2-2, -2.5, lastSize * 0.5-2)));
			bodymtx->scale(floorNum, 5, floorNum-1);
			bodymtx->addChild(body);
			house->addChild(bodymtx);
		}
		else {
		
		}
		roofmtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, -7 + floorNum, 0)));
		roofmtx->scale(lastSize+1, lastSize+1, lastSize+1);
		roofmtx->addChild(roof);

		house->addChild(roofmtx);
		house->addChild(fl);
		buildMap->addChild(house);
	}


	
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(skybox);
	glDeleteProgram(skyboxShaderProgram);
	glDeleteProgram(shaderProgram);
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
	P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{

}

void Window::display_callback(GLFWwindow* window)
{


	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Use the shader of programID
	glUseProgram(skyboxShaderProgram);
	skybox->draw(skyboxShaderProgram);

	glUseProgram(mapShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(mapShaderProgram, "view"), 1, GL_FALSE, &V[0][0]);
	glUniform3fv(glGetUniformLocation(mapShaderProgram, "cameraPos"), 1, &cam_pos[0]);
	car->draw(mapShaderProgram, glm::mat4(1.0f));

	glUseProgram(shaderProgram);
	buildMap->draw(shaderProgram, glm::mat4(1.0f));
	
	glUseProgram(shaderProgram);
	light_ptr = &dir_light;
	light_ptr->draw(shaderProgram);
	

	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);

	int n = 200;
	int i = 0;

	while (true) {
		if (up == true) {
			double preTime = glfwGetTime();
			car->translate(0, 0, 0.1);
			double currTime = glfwGetTime();
			float timestep = currTime - preTime;

			glUseProgram(mapShaderProgram);
			glUniformMatrix4fv(glGetUniformLocation(mapShaderProgram, "view"), 1, GL_FALSE, &V[0][0]);
			glUniform3fv(glGetUniformLocation(mapShaderProgram, "cameraPos"), 1, &cam_pos[0]);
			car->draw(mapShaderProgram, glm::mat4(1.0f));

			
			/*
			glUseProgram(shaderProgram);
			light_ptr = &dir_light;
			light_ptr->draw(shaderProgram);
			*/
			if (i == 0)
				glAccum(GL_LOAD, 1.0 / n);
			else
				glAccum(GL_ACCUM, 1.0 / n);

			i++;

			if (i >= n) {
				i = 0;
				glAccum(GL_RETURN, 1.0);
				// Gets events, including input such as keyboard and mouse or window resizing
				glfwPollEvents();
				// Swap buffers
				glfwSwapBuffers(window);
				//wait_until_next(timestep);
			}

		}

		if (up == false) {
			break;
		}
	}
	
	//std::cout << Window::robotNum << std::endl;
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

		if (key == GLFW_KEY_UP) {
			cout << "up" << endl;
			if (up == false) {
				up = true;
			}
			else {
				up = false;
			}
		}

	}
}



glm::vec3 Window::trackBallMapping(glm::vec2 point)
{
	glm::vec3 v;  
	float d;    
	v.x = (2.0*point.x - Window::width) / Window::width;   
	v.y = (Window::height - 2.0*point.y) / Window::height;   
	v.z = 0.0; 
	d = glm::length(v);
	d = (d<1.0) ? d : 1.0;
	v.z = sqrtf(1.001 - d*d);  
	v = glm::normalize(v);
	return v; 
}



void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	cursor_pos = { xpos,ypos };
	//cout << "curr:" << cursor_pos.x << "+" << cursor_pos.y << endl;
	float angle;


	switch (mouse) {
		case 1: {
			glm::vec3 curPoint;
			glm::vec3 prePoint;
			currPos = cursor_pos;
			curPoint = trackBallMapping(currPos);
			prePoint = trackBallMapping(prePos);
			float dot = glm::dot(curPoint, prePoint);

			if (dot > 1) {
				dot = 1;
			}
			float deg = acos(dot);
			glm::vec3 axis = glm::cross(prePoint, curPoint);
			cam_pos = glm::rotate(glm::mat4(1.0f), deg, axis)*glm::vec4(cam_pos, 1.0);
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
			break;
		}





	}

	prePos = currPos;
}

void Window::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	
	cam_pos = glm::translate(glm::mat4(1.0f), glm::vec3 (0,0, 0.5 * yoffset))*(glm::vec4(cam_pos, 1.0));
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	if (action == GLFW_PRESS) {
		
		prePos = cursor_pos;
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			mouse = 1;

		}

		
	}
	else if (action == GLFW_RELEASE){
		mouse = 0;
	}
}




