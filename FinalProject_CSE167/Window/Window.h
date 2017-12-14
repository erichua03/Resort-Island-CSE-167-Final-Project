#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

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

#include "shader.h"
#include "Skybox.h"
#include "Terrain.h"
#include "Transform.h"
#include "Plant.h"
#include "Bezier.h"
#include "OBJObject.h" // bunny debugging

using namespace std;

class Window
{
public:
    static GLint shaderProgram_skybox;
    static GLint shaderProgram_terrain;
    static GLint shaderProgram_plant;
    static GLint shaderProgram_house;
    static GLint shaderProgram_obj; // bunny debugging
    
    static glm::vec3 cam_pos;        // e  | Position of camera
    static glm::vec3 cam_look_at;    // d  | This is where the camera looks at
    static glm::vec3 cam_up;            // up | What orientation "up" is
    
	static int width;
	static int height;
    
    static double xpos;
    static double ypos;
    static double OLD_XPOS;
    static double OLD_YPOS;
    static double scrollY;
    
    static glm::vec3 A;
    static glm::vec3 B;
    static float angle;
    static glm::vec3 axis;
    
    static glm::mat4 terrainMat;
    
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
    static bool CAMERA_ROTATE;
    static bool TERRAIN_ROTATE;
    static bool TERRAIN_UP;
    static bool TERRAIN_DOWN;
    static bool TERRAIN_LEFT;
    static bool TERRAIN_RIGHT;
    static bool TERRAIN_SWITCH;
    
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
    static void update();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    //
    static glm::vec3 trackBallMapping(GLFWwindow* window, double xpos, double ypos);
    
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif
