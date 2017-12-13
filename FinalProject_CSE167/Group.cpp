#include "Group.h"
#include "Geometry.h"
#include "Window.h"
#include <iostream>
#include <algorithm>

using namespace std;
Group::Group(char * name)
{
	
}
Group::Group()
{
	sphere = new Geometry("../sphere.obj");
	sphere->scale(2.5f, 2.5f, 2.5f);
	//bounding = false;
}
Group::~Group()
{

}

void Group::draw(GLuint shaderProgram, glm::mat4 C) {

	
	glm::vec4 origin = Window::P * Window::V * C * glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 5.0f))* glm::vec4(0.3, 0, 0, 1);
	glm::vec3 t = origin / origin.w;
	glm::mat4 inverseCameraRotation = Window::V;
	inverseCameraRotation[3] = glm::vec4(0, 0, 0, 1);
	glm::mat4 cameraRotation = transpose(inverseCameraRotation);

	glm::vec4 axis = cameraRotation * glm::vec4(-1, 0, 0, 0);
	glm::vec4 theOtherSideInWorld = glm::vec4(0.3, 0, 0, 1) + axis *5.0f;
	glm::vec4 theOtherSide = Window::P * Window::V * C * theOtherSideInWorld;
	glm::vec3 side = theOtherSide / theOtherSide.w;
	float lrad = glm::length(side - t);
	//cout << "left:" <<lrad << endl;

	glm::vec4 Laxis = cameraRotation * glm::vec4(1, 0, 0, 0);
	glm::vec4 LtheOtherSideInWorld = glm::vec4(0.3, 0, 0, 1) + Laxis *5.0f;
	glm::vec4 LtheOtherSide = Window::P * Window::V * C * LtheOtherSideInWorld;
	glm::vec3 Lside = LtheOtherSide / LtheOtherSide.w;
	float rrad = glm::length(Lside - t);
	//cout << "right:" << rrad << endl;

	axis = cameraRotation * glm::vec4(0, 1, 0, 0);
	theOtherSideInWorld = glm::vec4(0.3, 0, 0, 1) + axis *5.0f;
	theOtherSide = Window::P * Window::V * C * theOtherSideInWorld;
	side = theOtherSide / theOtherSide.w;
	float urad = glm::length(side - t);
	//cout << "up:" << urad << endl;

	axis = cameraRotation * glm::vec4(0, -1, 0, 0);
	theOtherSideInWorld = glm::vec4(0.3, 0, 0, 1) + axis *5.0f;
	theOtherSide = Window::P * Window::V * C * theOtherSideInWorld;
	side = theOtherSide / theOtherSide.w;
	float drad = glm::length(side - t);
	//cout << "down:" << drad << endl;

	axis = cameraRotation * glm::vec4(0, 0, 1, 0);
	theOtherSideInWorld = glm::vec4(0.3, 0, 0, 1) + axis *5.0f;
	theOtherSide = Window::P * Window::V * C * theOtherSideInWorld;
	side = theOtherSide / theOtherSide.w;
	float frad = glm::length(side - t);
	//cout << "front:" << frad << endl;

	axis = cameraRotation * glm::vec4(0, 0, -1, 0);
	theOtherSideInWorld = glm::vec4(0.3, 0, 0, 1) + axis *5.0f;
	theOtherSide = Window::P * Window::V * C * theOtherSideInWorld;
	side = theOtherSide / theOtherSide.w;
	float brad = glm::length(side - t);
	//cout << "back:" << brad << endl;

}

void Group::addChild(Node * child)
{
	children.push_back(child);
}

void Group::removeChild(Node * child)
{
	std::vector<Node*>::iterator position = std::find(children.begin(), children.end(), child);
	if (position != children.end()) {
		children.erase(position);
	}
}

void Group::update()
{
}


