#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>
#include <vector>

class EBO
{
public:
	// ID de reference de l'EBO
	GLuint ID;
	// constructor qui genere un objet buffer d'elements et le relie à ses indices
	EBO(GLuint* indices, GLsizeiptr size);
	EBO(std::vector<GLuint>& indices);


	void Bind();

	void Unbind();

	void Delete();
};

#endif