#include"VAO.h"

// constructeur generant un ID pour le VAO
VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

// Links a VBO to the VAO using a certain layout
void VAO::LinkVBO(VBO& VBO, GLuint layout)
{
	VBO.Bind();
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

// Bind le VAO , dit a opengl que c'est cette objet qu'on utilise au prochain drawcall
void VAO::Bind()
{
	glBindVertexArray(ID);
}

// pour unbind on dit a opengl d'utiliser l'element 0 qui sert d'element null
void VAO::Unbind()
{
	glBindVertexArray(0);
}

// supprime d'openGL nos données concernant ce VAO et libere automatiquement la memoire
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}