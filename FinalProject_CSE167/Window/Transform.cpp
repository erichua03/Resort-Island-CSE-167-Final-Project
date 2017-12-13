#include "Transform.h"


Transform::Transform(glm::mat4 m) {
	M = m;
}

Transform::~Transform() {
	
}

void Transform::draw(GLuint shaderProgram,glm::mat4 C) {
	
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->draw(shaderProgram,C*M);
	}
}

void Transform::addChild(Node* child) {
	children.push_back(child);
}

void Transform::removeChild(Node* child) {
	std::vector<Node*>::iterator position = std::find(children.begin(), children.end(),child);
	if (position != children.end()) {
		children.erase(position);
	}
}

void Transform::update()
{
}

void Transform::rotate(float deg, glm::vec3 axis) {
	this->M = glm::rotate(glm::mat4(1.0f), deg, axis) * M;
}

void Transform::translate(float x, float y, float z) {

	this->M = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) * M;
}

void Transform::scale(float x, float y, float z) {
	this->M =  M * glm::scale(glm::mat4(1.0f), glm::vec3(x,y,z));
}
