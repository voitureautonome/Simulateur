#ifndef LINE_H
#define LINE_H
#include <imgui.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <vector>
#include "glm/vec2.hpp"
class Line {
public:
	static std::vector<GLfloat> listeLignes;
	static std::vector<GLuint> listeIndices;
	static int number;
	static void initLines();
	static int drawLine(glm::vec2 start, glm::vec2 end, ImVec4 color);
};
//int LinesNumbers;
//GLuint* linesIndices;
#endif