#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <cmath>
#include "Model.h"
#include "VBO.h"



struct triangle
{
	glm::vec3 angle[3];
};


bool rayCast(glm::vec2 wp1, glm::vec2 wp2, glm::vec2 rayStart, glm::vec2 rayDirection, glm::vec2& res);
float distance(glm::vec2 A, glm::vec2 B);
float rayCastTriangle(triangle angle, glm::vec2 rayStart, glm::vec2 rayDirection);
void simuLidar(Model laby, Model voit);

int testRaycast();