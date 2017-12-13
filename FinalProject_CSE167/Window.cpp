#include "Window.h"
using namespace std;
const char* window_title = "GLFW Starter Project";
Cube * cube;
Plant * plant;
Transform * flag;
Transform * stickTrans;
Geometry * stick;
Geometry * bezierPach;

GLint shaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "/Users/dorishe/Desktop/CSE167StarterCode2/shaderPlant.vert"
#define FRAGMENT_SHADER_PATH "/Users/dorishe/Desktop/CSE167StarterCode2/shaderPlant.frag"

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
// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 200.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is
glm::vec3 cam_pos2 = cam_pos;
glm::mat4 cam_rot_M, cam_rot_M2;
glm::vec3 lastPoint, curPoint;
int clickFlag = 0;
int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;
int Window::layers;

void Window::initialize_objects()
{
	cube = new Cube();
    plant = new Plant();
    flag=new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(20.0f,0.0f,0.0f)));
    bezierPach = new Geometry(bezierPoints,0.1f,0.1f,0.8f);
    stick= new Geometry(plant->vertices,plant->indices,0.5f,0.5f,0.5f);
    stickTrans=new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,45.0f,0.0f))*glm::scale(glm::mat4(1.0f), glm::vec3(1.0f,4.0f,1.0f))*glm::rotate(glm::mat4(1.0f),90/ 180.0f * glm::pi<float>(), glm::vec3(1.5f, 0.0f, 1.0f)));
    stickTrans->addChild(stick);
    flag->addChild(stickTrans);
    flag->addChild(bezierPach);
                             
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
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
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    V = glm::lookAt(glm::vec3(cam_rot_M*glm::vec4(cam_pos, 0.0f)), cam_look_at, cam_up);
	// Use the shader of programID
	glUseProgram(shaderProgram);
	
	// Render the cube
	//cube->draw(shaderProgram);
    layers=3;
    
    std::string rules[2]={"AABBAA","BBA"};
    plant->draw(shaderProgram,layers,glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f),'A',rules);
    flag->draw(V);
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
        float offset = cam_pos.z > 0 ? 5 : -5;
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
        else if (key == GLFW_KEY_S) {
            cam_pos = glm::vec3(cam_pos.x, cam_pos.y, cam_pos.z + offset);
        }
        else if (key == GLFW_KEY_W) {
            if (abs(cam_pos.z) > 5) {
                cam_pos = glm::vec3(cam_pos.x, cam_pos.y, cam_pos.z - offset);
            }
        }
        else if (key==GLFW_KEY_EQUAL){
            plant->update(1);
        }
        else if (key==GLFW_KEY_MINUS){
            plant->update(-1);
        }
	}
}
void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        cout<<"click"<<endl;
        clickFlag = 1;
        double tempX, tempY;
        glfwGetCursorPos(window, &tempX, &tempY);
        auto point = glm::vec2(tempX - width / 2, -tempY + height / 2);
        lastPoint = trackBallMapping(point);
        cam_rot_M2 = cam_rot_M;
        glMatrixMode(GL_MODELVIEW);
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT  && action == GLFW_RELEASE) {
        clickFlag = 0;
        cam_rot_M2 = cam_rot_M;
    }
}
void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (clickFlag == 1) {
        auto point = glm::vec2(xpos - width / 2, -ypos + height / 2);
        //cout << point.x << point.y << endl;
        curPoint = trackBallMapping(point);
        //cout << curPoint.x << curPoint.y << curPoint.z << endl;
        auto direction = glm::vec3(curPoint.x - lastPoint.x, curPoint.y - lastPoint.y, curPoint.z - lastPoint.z);
        float velocity = glm::length(direction);
        if (velocity > 0.0001) {
            glm::vec3 rotAxis = glm::vec3(cam_rot_M*glm::vec4(glm::cross(lastPoint, curPoint),1.0f));
            //glm::vec3 new_axis = glm::vec3(V*glm::vec4(rotAxis,1.0f));
            //cout <<':'<< rotAxis.x << rotAxis.y << rotAxis.z << endl;
            auto rot_angle = -velocity * 60.0f / 180.0f * glm::pi<float>();
            auto rotM = glm::rotate(glm::mat4(1.0f), rot_angle, rotAxis);
            cam_rot_M = rotM*cam_rot_M2;
        }
    }
}
glm::vec3 Window::trackBallMapping(glm::vec2 point) {
    glm::vec3 v;
    float d;
    v.x = 2 * point.x / width;
    v.y = 2 * point.y / height;
    v.z = 0.0f;
    d = sqrtf(v.x*v.x + v.y*v.y);
    d = (d < 1.0) ? d : 1.0;
    v.z = sqrtf(1.001 - d*d);
    glm::normalize(v); // Still need to normalize, since we only capped d, not v.
    return v;
}
