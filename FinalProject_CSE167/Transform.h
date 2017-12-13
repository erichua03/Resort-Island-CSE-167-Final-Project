#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Node.h"
#include "Group.h"
#include <vector>


class Transform: public Group {
	public:

		glm::mat4 M;
		glm::vec3 color;
		std::vector<Node*> children;
		Transform(glm::mat4 m);
		~Transform();
		void draw(GLuint shaderProgram,glm::mat4 C) ;
		void addChild(Node* child);
		void removeChild(Node* child);
		void update() ;
		void rotate(float deg, glm::vec3 axis);
		void translate(float x, float y, float z);
		void Transform::scale(float x, float y, float z);
 };
#endif