#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <cmath>
#include "VBO.h"


//bool TestRayOBBIntersection(
//	glm::vec3 ray_origin,        // Ray origin, in world space
//	glm::vec3 ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
//	glm::vec3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
//	glm::vec3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
//	glm::mat4 ModelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
//	float& intersection_distance // Output : distance between ray_origin and the intersection with the OBB
//);

//glm::vec3 raycastCarre(
//	glm::vec3& rayOrigin, // Origine du rayon
//	glm::vec3& rayDirection, // Direction du rayon
//	glm::vec3* mursVertices, // Tableau contenant les 2 vertices de chaque mur (MUR1 pointA [0] pointB [1], MUR2 pointA [2] pointB [3]...)
//	int nbMurs, // Nombre de murs pour la taille des tableaux
//	glm::mat4* modelMatrix // Tableau des matrices représentant chaque mur
//);

//glm::vec3 triIntersect(glm::vec3 ro, glm::vec3 rd, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
//
//glm::vec3 raycastTri(glm::mat4 model);

//int test();

bool rayCast(glm::vec2 wp1, glm::vec2 wp2, glm::vec2 rayStart, glm::vec2 rayDirection, glm::vec2& res);
float distance(glm::vec2 A, glm::vec2 B);
void rayCastTriangle(Vertex* triangle, glm::vec2 rayStart, glm::vec2 rayDirection);


int test2(glm::mat4 model);