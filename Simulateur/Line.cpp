#include "Line.h"
#include "glm/vec2.hpp"
std::vector<GLfloat> Line::listeLignes;
std::vector<GLuint> Line::listeIndices;

int Line::number = 0;

void Line::initLines()
{
	Line::number = 0;
	Line::listeIndices.clear();
	Line::listeIndices.clear();
	for (size_t i = 0; i < 1000; i++)
	{
		Line::listeIndices.push_back(i);
	}
}

int Line::drawLine(glm::vec2 start, glm::vec2 end, ImVec4 color)
{
	Line::listeLignes.push_back(start[0]);
	Line::listeLignes.push_back(start[1]);
	Line::listeLignes.push_back(0.f);
	Line::listeLignes.push_back(end[0]);
	Line::listeLignes.push_back(end[1]);
	Line::listeLignes.push_back(0.f);
	Line::number++;
	return 0;
}
