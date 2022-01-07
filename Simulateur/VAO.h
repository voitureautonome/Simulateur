#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
public:
	// ID reference de l'objet V AO
	GLuint ID;
	// constructeur generant un ID pour le VAO
	VAO();

	void LinkVBO(VBO& VBO, GLuint layout);

	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	// Bind le VAO
	void Bind();
	// Unbind le VAO
	void Unbind();
	// Supprime le VAO
	void Delete();
};
#endif