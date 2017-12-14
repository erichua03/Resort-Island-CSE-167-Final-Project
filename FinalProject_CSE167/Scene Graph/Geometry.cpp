#include "Geometry.h"
#include "Window.h"
#include "light.h"
#include "Node.h"
#include <iostream>


using namespace std;

Geometry::Geometry(const char * filePath, int x) {
	toWorld = glm::mat4(1.0f);
	parseLight(filePath);


}

Geometry::Geometry(const char * filePath)
{
	toWorld = glm::mat4(1.0f);
	parse(filePath);
	this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(-avgX, -avgY, -avgZ)) * toWorld;
	toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(1 / size)) * toWorld;


	for (int i = 0; i < normals.size(); i++) {
		normals[i] = glm::normalize(normals[i]);
	}
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);
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
					 // Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
					 // you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.

	//glm::vec3 normals = glm::normalize(normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normals.size(), normals.data(), GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

					 // We've sent the vertex data over to OpenGL, but there's still something missing.
					 // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);

}

void Geometry::drawLight(GLuint shaderProgram, light light) {
	glm::mat4 modelview = Window::V * light.lightToWorld *glm::translate(glm::mat4(1.0f), light.light_pos);
	lProjection = glGetUniformLocation(shaderProgram, "projection");
	lModelview = glGetUniformLocation(shaderProgram, "modelview");
	glUniformMatrix4fv(lProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(lModelview, 1, GL_FALSE, &modelview[0][0]);


	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &LVAO);
	glGenBuffers(1, &LVBO);
	glGenBuffers(1, &LEBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(LVAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, LVBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*lightVertices.size(), lightVertices.data(), GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*lightIndices.size(), lightIndices.data(), GL_STATIC_DRAW);
	cout << lightIndices.size() << endl;
	cout << lightVertices.size() << endl;
	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.

	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	//cout << "draw light" << endl;
	// Now draw the cube. We simply need to bind the VAO associated with it.
	//glBindVertexArray(LVAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, (GLsizei)lightIndices.size(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void Geometry::parseLight(const char *filepath) {
	FILE* fp;     // file pointer

	float x, y, z;  // vertex coordinates
	float r, g, b;  // vertex color
	int c1 = 0, c2 = 0;    // characters read from file
	GLuint f1, f2, f3, f4, f5, f6;

	fp = fopen(filepath, "rb");

	while (c1 != EOF || c2 != EOF) {
		c1 = fgetc(fp);
		c2 = fgetc(fp);

		if ((c1 == 'v') && (c2 == ' '))
		{
			fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			lightVertices.push_back(glm::vec3(x, y, z));
		}

		else if ((c1 == 'f') && (c2 == ' ')) {
			fscanf(fp, "%u//%u %u//%u %u//%u", &f1, &f2, &f3, &f4, &f5, &f6);
			lightIndices.push_back(f1 - 1);
			lightIndices.push_back(f3 - 1);
			lightIndices.push_back(f5 - 1);
		}
		else {
			ungetc(c2, fp);
		}
	}

	fclose(fp);
}

void Geometry::parse(const char *filepath)
{
	FILE* fp;     // file pointer

	float x, y, z;  // vertex coordinates
	float r, g, b;  // vertex color
	int c1 = 0, c2 = 0;    // characters read from file
	GLuint f1, f2, f3, f4, f5, f6, f7, f8;

	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float minZ = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float maxY = std::numeric_limits<float>::min();
	float maxZ = std::numeric_limits<float>::min();

	fp = fopen(filepath, "rb");  // make the file name configurable so you can load other files
    if (fp == NULL) { cerr << "error loading file: " << filepath << endl; exit(-1); }  // just in case the file can't be found or is corrupt



																		 // go through the obj file line by line until it reaches the end of line
	while (c1 != EOF || c2 != EOF) {

		c1 = fgetc(fp);
		c2 = fgetc(fp);
		// if it is a vertex
		if ((c1 == 'v') && (c2 == ' '))
		{
			fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			vertices.push_back(glm::vec3(x, y, z));


			if (x > maxX) {
				maxX = x;
			}

			else if (x < minX) {
				minX = x;
			}

			if (y > maxY) {
				maxY = y;
			}

			else if (y < minY) {
				minY = y;
			}

			if (z > maxZ) {
				maxZ = z;
			}

			else if (z < minZ) {
				minZ = z;
			}
		}

		// if it is a normal
		else if ((c1 == 'v') && (c2 == 'n'))
		{
			fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			normals.push_back(glm::vec3(x, y, z));
		}
		else if (((c1 == 'f') && (c2 == ' '))) {
			fscanf(fp, "%u//%u %u//%u %u//%u", &f1, &f2, &f3, &f4, &f5, &f6);
			indices.push_back(f1 - 1);
			indices.push_back(f3 - 1);
			indices.push_back(f5 - 1);
		}

		else {
			ungetc(c2, fp);
		}
	}

	avgX = (maxX + minX) / 2.0f;
	avgY = (maxY + minY) / 2.0f;
	avgZ = (maxZ + minZ) / 2.0f;

	size = std::max((maxX - minX) / 2.0f, (maxY - minY) / 2.0f);
	size = std::max((maxZ - minZ) / 2.0f, size);

	fclose(fp);   // make sure you don't forget to close the file when done
}

Geometry::~Geometry() {
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO2);
}

void Geometry::draw(GLuint shaderProgram, glm::mat4 C)
{
	
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * (C * toWorld);
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");
	// MATERIAL
	uDiffuse = glGetUniformLocation(shaderProgram, "material.diffuse");
	uSpecular = glGetUniformLocation(shaderProgram, "material.specular");
	uAmbient = glGetUniformLocation(shaderProgram, "material.ambient");
	uShininess = glGetUniformLocation(shaderProgram, "material.shininess");
	uRenderMode = glGetUniformLocation(shaderProgram, "material.obj_mode");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &C[0][0]);
	glUniform3f(uAmbient, ambient.x, ambient.y, ambient.z);
	glUniform3f(uDiffuse, diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(uSpecular, specular.x, specular.y, specular.z);
	glUniform1f(uShininess, shininess);
	glUniform1i(uRenderMode, renderMode);
	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
	

	/*
	glm::mat4 modelview = Window::V * (C * toWorld);
	GLuint uProjection = glGetUniformLocation(shaderProgram, "projection");
	GLuint uModelview = glGetUniformLocation(shaderProgram, "modelview");

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &C[0][0]);
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	*/
}

void Geometry::update()
{
	//spin(1.0f);
}

void Geometry::spin(float deg)
{
	// If you haven't figured it out from the last project, this is how you fix spin's behavior
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), 1.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Geometry::rotate(float deg, glm::vec3 axis) {
	toWorld = glm::rotate(glm::mat4(1.0f), deg, axis) * this->toWorld;
}

void Geometry::translate(float x, float y, float z) {

	this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) * toWorld;
}

void Geometry::scale(float x, float y, float z) {
	this->toWorld = this->toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(x,y,z));
}
