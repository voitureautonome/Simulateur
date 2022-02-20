#ifndef LINE_H
#define LINE_H
#include <imgui.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <vector>
#include "glm/vec3.hpp"
#include "Camera.h"
#include "shaderClass.h"
#include "VAO.h"
class Line {
public:
	Line();
	int number;
	int count = 0;
	VAO *VAO2;
	VBO *VBO2;
	int drawLine(glm::vec3 start, glm::vec3 end);
	void Draw(Camera& camera);
	void Clear();
	Shader shaderLigne = Shader("ligne.vert", "ligne.frag");
	GLfloat* lignes;
};
#endif