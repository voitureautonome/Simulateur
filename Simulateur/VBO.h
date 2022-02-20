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
	//ID reference de l'objet
	GLuint ID;
	std::vector<GLfloat>* vector;
	
	//Cnstruit un VBO et le relie a ses sommets
	VBO(GLfloat* vertices, GLsizeiptr size);
	VBO(std::vector<GLfloat>* _vector, GLsizeiptr size);
	VBO(std::vector<GLfloat> _vector, GLsizeiptr size);
	VBO(std::vector<Vertex>& vertices);

	void updateData(GLintptr offset, GLuint noElements, GLfloat* data);

	// Bind le VBO
	void Bind();
	// Unbind le VBO
	void Unbind();
	// Supprime le VBO
	void Delete();
};

#endif