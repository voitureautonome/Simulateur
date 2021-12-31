#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>
#include <stdlib.h>
#include <vector>
#include<glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};

class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	std::vector<GLfloat>* vector;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO(GLfloat* vertices, GLsizeiptr size);
	VBO(std::vector<GLfloat>* _vector, GLsizeiptr size);
	VBO(std::vector<Vertex>& vertices);

	// Binds the VBO
	void Bind();
	// Unbinds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();
};

#endif