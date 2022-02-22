#include "Line.h"
#include "glm/vec2.hpp"
#define NOMBREFLOATS 3000
Line::Line()
{
	this->number = 0;
	this->VAO2 = new VAO();
	this->VAO2->Bind();
	this->lignes = (GLfloat*)malloc(NOMBREFLOATS * sizeof(GLfloat));

	this->VBO2 = new VBO(lignes, (long long int) (NOMBREFLOATS * sizeof(GLfloat)));
	this->VAO2->Bind();
	this->VAO2->LinkVBO(*VBO2, 0);
	this->VAO2->Unbind();
	this->VBO2->Unbind();
}

int Line::drawLine(glm::vec3 start, glm::vec3 end)
{
	this->number++;
	for (unsigned int i = 0; i < 3; i++)
	{
		lignes[count] = start[i];
		count++;
	}
	for (unsigned int i = 0; i < 3; i++)
	{
		lignes[count] = end[i];
		count++;
	}
	this->VBO2->Bind();
	/* On a modifié notre tableau dans la mémoire du CPU mais on doit mettre a jour la copie dans la mémoire du GPU
	*  on pourrait mettre a jour que la partie qui a changé mais pour rester simple on recopie tout 
	*  cela reste rapide car on ne le fait qu'a chaque modification et non a chaque drawCall
	*/
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->number * (int) 3 *sizeof(GLfloat) * sizeof(GLfloat), lignes);
	return 0;
}

void Line::Draw(Camera& camera)
{
	this->VAO2->Bind();
	shaderLigne.Activate();

	glUniform3f(glGetUniformLocation(shaderLigne.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.Matrix(shaderLigne, "camMatrix");

	glDrawArrays(GL_LINES, 0, Line::number * 2);
	this->VAO2->Unbind();
}
void Line::Clear() {
	this->VBO2->Bind();
	for (size_t i = 0; i < NOMBREFLOATS; i++)
	{
		lignes[i] = 0;
	}
	/* on renvoit dans la memoire du GPU notre VBO rempli de 0 
	*/
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->number * 3 * sizeof(GLfloat) * sizeof(GLfloat), lignes);
	this->number = 0;
	this->count = 0;
	this->VBO2->Unbind();
}