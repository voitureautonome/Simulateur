#include "raycast.h"


//
//bool TestRayOBBIntersection(
//	glm::vec3 ray_origin,        // Ray origin, in world space
//	glm::vec3 ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
//	glm::vec3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
//	glm::vec3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
//	glm::mat4 ModelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
//	float& intersection_distance // Output : distance between ray_origin and the intersection with the OBB
//) {
//
//	// Intersection method from Real-Time Rendering and Essential Mathematics for Games
//
//	float tMin = 0.0f;
//	float tMax = 100000.0f;
//
//	glm::vec3 OBBposition_worldspace(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);
//
//	glm::vec3 delta = OBBposition_worldspace - ray_origin;
//
//	// Test intersection with the 2 planes perpendicular to the OBB's X axis
//	{
//		glm::vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
//		float e = glm::dot(xaxis, delta);
//		float f = glm::dot(ray_direction, xaxis);
//
//		if (fabs(f) > 0.001f) { // Standard case
//
//			float t1 = (e + aabb_min.x) / f; // Intersection with the "left" plane
//			float t2 = (e + aabb_max.x) / f; // Intersection with the "right" plane
//			// t1 and t2 now contain distances betwen ray origin and ray-plane intersections
//
//			// We want t1 to represent the nearest intersection, 
//			// so if it's not the case, invert t1 and t2
//			if (t1 > t2) {
//				float w = t1; t1 = t2; t2 = w; // swap t1 and t2
//			}
//
//			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
//			if (t2 < tMax)
//				tMax = t2;
//			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
//			if (t1 > tMin)
//				tMin = t1;
//
//			// And here's the trick :
//			// If "far" is closer than "near", then there is NO intersection.
//			// See the images in the tutorials for the visual explanation.
//			if (tMax < tMin)
//				return false;
//
//		}
//		else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
//			if (-e + aabb_min.x > 0.0f || -e + aabb_max.x < 0.0f)
//				return false;
//		}
//	}
//
//
//	// Test intersection with the 2 planes perpendicular to the OBB's Y axis
//	// Exactly the same thing than above.
//	{
//		glm::vec3 yaxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);
//		float e = glm::dot(yaxis, delta);
//		float f = glm::dot(ray_direction, yaxis);
//
//		if (fabs(f) > 0.001f) {
//
//			float t1 = (e + aabb_min.y) / f;
//			float t2 = (e + aabb_max.y) / f;
//
//			if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }
//
//			if (t2 < tMax)
//				tMax = t2;
//			if (t1 > tMin)
//				tMin = t1;
//			if (tMin > tMax)
//				return false;
//
//		}
//		else {
//			if (-e + aabb_min.y > 0.0f || -e + aabb_max.y < 0.0f)
//				return false;
//		}
//	}
//
//
//	// Test intersection with the 2 planes perpendicular to the OBB's Z axis
//	// Exactly the same thing than above.
//	{
//		glm::vec3 zaxis(ModelMatrix[2].x, ModelMatrix[2].y, ModelMatrix[2].z);
//		float e = glm::dot(zaxis, delta);
//		float f = glm::dot(ray_direction, zaxis);
//
//		if (fabs(f) > 0.001f) {
//
//			float t1 = (e + aabb_min.z) / f;
//			float t2 = (e + aabb_max.z) / f;
//
//			if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }
//
//			if (t2 < tMax)
//				tMax = t2;
//			if (t1 > tMin)
//				tMin = t1;
//			if (tMin > tMax)
//				return false;
//
//		}
//		else {
//			if (-e + aabb_min.z > 0.0f || -e + aabb_max.z < 0.0f)
//				return false;
//		}
//	}
//
//	intersection_distance = tMin;
//	return true;
//
//}


//glm::vec3 raycastCarre(
//	glm::vec3 &rayOrigin, // Origine du rayon
//	glm::vec3 &rayDirection, // Direction du rayon
//	glm::vec3 *mursVertices, // Tableau contenant les 2 vertices de chaque mur (MUR1 pointA [0] pointB [1], MUR2 pointA [2] pointB [3]...)
//	int nbMurs, // Nombre de murs pour la taille des tableaux
//	glm::mat4 *modelMatrix // Tableau des matrices représentant chaque mur
//	) 
//{
//	float minDist = 10000.f;
//	// On parcourt le tableau de murs
//	for (int i = 0; i < nbMurs; i++)
//	{
//		float dist = 10000.f;
//		bool res = TestRayOBBIntersection(rayOrigin, rayDirection, mursVertices[i * 2], mursVertices[i * 2 + 1], modelMatrix[i], dist);
//		if (minDist > dist)
//		{
//			minDist = dist;
//		}
//	}
//	std::cout << minDist << std::endl;
//	glm::vec3 pos = rayOrigin + rayDirection * minDist;
//	return pos;
//}

//int test() {
//	glm::vec3 origine = glm::vec3(0.f, 0.f, 0.f);
//	glm::vec3 direction = glm::vec3(1.5f, 0.f, 1.f);
//	std::cout << glm::to_string(glm::normalize(direction)) << std::endl;
//	glm::vec3 direcNormalise = glm::normalize(direction);
//
//	glm::vec3 mur1A = glm::vec3(3.f, 0.f, 1.f);
//	glm::vec3 mur1B = glm::vec3(3.f, 0.f, 5.f);
//	glm::vec3 mur2A = glm::vec3(6.f, 1.f, 0.f);
//	glm::vec3 mur2B = glm::vec3(6.f, 10.f, 0.f);
//
//	glm::vec3 tab[4];
//	tab[0] = mur1A;
//	tab[1] = mur1B;
//	tab[2] = mur2A;
//	tab[3] = mur2B;
//
//	glm::mat4 matrix1 = glm::mat4(1.f);
//	glm::mat4 matrix2 = glm::mat4(2.f);
//	glm::mat4 mat[2];
//	mat[0] = matrix1;
//	mat[1] = matrix2;
//
//	glm::vec3 pos = raycastCarre(origine, direcNormalise, tab, 2, mat);
//
//	std::cout << glm::to_string(pos)<<std::endl;
//
//	return 0;
//}

//glm::vec3 triIntersect(glm::vec3 ro, glm::vec3 rd, Vertex* triangle, glm::mat4 model)
//{
//	// On applique le modèle du monde au triangle pour obtenir les coordonées des sommets dans le monde
//	glm::vec4 vec41 = model * glm::vec4(triangle[0].position, 1.0);
//	glm::vec3 vec31 = glm::vec3(vec41);
//	glm::vec4 vec42 = model * glm::vec4(triangle[1].position, 1.0);
//	glm::vec3 vec32 = glm::vec3(vec42);
//	glm::vec4 vec43 = model * glm::vec4(triangle[2].position, 1.0);
//	glm::vec3 vec33 = glm::vec3(vec43);
//	
//	std::cout << "Ray Origine : " << (glm::to_string(ro)) << std::endl;
//	std::cout << "Ray Direction : " << (glm::to_string(rd)) << std::endl;
//	std::cout << "Sommet1 : " << (glm::to_string(vec31)) << std::endl;
//	std::cout << "Sommet2 : " << (glm::to_string(vec32)) << std::endl;
//	std::cout << "Sommet3 : " << (glm::to_string(vec33)) << std::endl;
//
//	glm::vec3 v1v0 = vec32 - vec31;
//	glm::vec3 v2v0 = vec33 - vec31;
//	glm::vec3 rov0 = ro - vec31;
//	std::cout << "roV0 : " << (glm::to_string(rov0)) << std::endl;
//	std::cout << "v1v0 : " << (glm::to_string(v1v0)) << std::endl;
//	std::cout << "v2V0 : " << (glm::to_string(v2v0)) << std::endl;
//
//
//	glm::vec3  n = glm::cross(v1v0, v2v0); // cross produit vectoriel
//	glm::vec3  q = glm::cross(rov0, rd);
//	std::cout << "cross v1v0, v2v0 : " << (glm::to_string(n)) << std::endl;
//	std::cout << "cross roV0, rd: " << (glm::to_string(q)) << std::endl;
//
//
//	float d = 1.0f / dot(rd, n); // dot produit scalaire
//	float u = d * dot(-q, v2v0);
//	float v = d * dot(q, v1v0)	;
//	float t = d * dot(-n, rov0);
//	if (u < 0.0f || u>1.0f || v < 0.0f || (u + v)>1.0f)
//		t = -1.0f;
//	//return glm::vec3(model * glm::vec4(glm::vec3(t, u, v),1.0f));
//	return glm::vec3(t, u, v);
//}

bool rayCast(glm::vec2 wp1, glm::vec2 wp2, glm::vec2 rayStart, glm::vec2 rayDirection, glm::vec2 &res) {
	const float x1 = wp1.x;
	const float y1 = wp1.y;
	const float x2 = wp2.x;
	const float y2 = wp2.y;

	const float x3 = rayStart.x;
	const float y3 = rayStart.y;
	const float x4 = rayStart.x + rayDirection.x;
	const float y4 = rayStart.y + rayDirection.y;

	const float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	if (den == 0) {
		return false;
	}

	const float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
	const float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
	if (t > 0 && t < 1 && u > 0) {
		res = glm::vec2(x1 + t * (x2 - x1), y1 + t * (y2 - y1));
	}
	else {
		return false;
	}
}

bool RayIntersectsTriangle(glm::vec3 rayOrigin,
	glm::vec3 rayVector,
	Vertex* inTriangle,
	glm::vec3& outIntersectionPoint)
{
	const float EPSILON = 0.0000001;
	glm::vec3 vertex0 = inTriangle[0].position;
	glm::vec3 vertex1 = inTriangle[1].position;
	glm::vec3 vertex2 = inTriangle[2].position;
	glm::vec3 edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	h = glm::cross(rayVector, edge2);
	a = glm::dot(edge1, h);
	if (a > -EPSILON && a < EPSILON)
		return false;    // Le rayon est parallèle au triangle.

	f = 1.0 / a;
	s = rayOrigin - vertex0;
	u = f * glm::dot(s, h);
	if (u < 0.0 || u > 1.0)
		return false;
	q = glm::cross(s, edge1);
	v = f * glm::dot(rayVector, q);
	if (v < 0.0 || u + v > 1.0)
		return false;

	// On calcule t pour savoir ou le point d'intersection se situe sur la ligne.
	float t = f * glm::dot(edge2, q);
	if (t > EPSILON) // Intersection avec le rayon
	{
		outIntersectionPoint = rayOrigin + rayVector * t;
		return true;
	}
	else // On a bien une intersection de droite, mais pas de rayon.
		return false;
}

float distance(glm::vec2 A, glm::vec2 B) {
	return sqrt(pow(B.x - A.x, 2) + pow(B.y - A.y, 2));
}

void rayCastTriangle(Vertex* triangle, glm::vec2 rayStart, glm::vec2 rayDirection) {
	
	glm::vec2 pointA = glm::vec2(triangle[0].position.x, triangle[0].position.z);
	glm::vec2 pointB = glm::vec2(triangle[1].position.x, triangle[1].position.z);
	glm::vec2 pointC = glm::vec2(triangle[2].position.x, triangle[2].position.z);

	glm::vec2 res = glm::vec2(-999.f, -999.f);
	glm::vec2 resAB, resAC, resBC;
	float longueur = 1000000.f;
	// On vérifie le côté du triangle AB
	if (rayCast(pointA, pointB, rayStart, rayDirection, resAB)) {
		float dist = distance(rayStart, resAB);
		std::cout << "resultat AB : " << glm::to_string(resAB) << std::endl;
		if (dist < longueur)
		{
			res = resAB;
			longueur = dist;
		}
	}
	// On vérifie le côté du triangle AC
	if (rayCast(pointA, pointC, rayStart, rayDirection, resAC)) {
		float dist = distance(rayStart, resAC);
		if (dist < longueur)
		{
			res = resAC;
			longueur = dist;
		}
	}
	// On vérifie le côté du triangle BC
	if (rayCast(pointB, pointC, rayStart, rayDirection, resBC)) {
		float dist = distance(rayStart, resBC);
		if (dist < longueur)
		{
			res = resBC;
			longueur = dist;
		}
	}
	std::cout << "resultat : " << glm::to_string(res) << std::endl;
	std::cout << "longueur : " << longueur << std::endl;

}

//glm::vec3 RayIntersectTri(glm::vec3 origine, glm::vec3 direction, Vertex* triangle, glm::mat4 model, glm::vec3& intersection) {
//	const float EPSILON = 0.0001;
//
//	glm::vec4 vec41 = model * glm::vec4(triangle[0].position, 1.0);
//	glm::vec3 vec31 = glm::vec3(vec41);
//	glm::vec4 vec42 = model * glm::vec4(triangle[1].position, 1.0);
//	glm::vec3 vec32 = glm::vec3(vec42);
//	glm::vec4 vec43 = model * glm::vec4(triangle[2].position, 1.0);
//	glm::vec3 vec33 = glm::vec3(vec43);
//
//	glm::vec3 cote1 = vec32 - vec31;
//	glm::vec3 cote2 = vec33 - vec31;
//
//	glm::vec3 h = glm::cross(direction, cote1);
//	glm::vec3 a = glm::cross(h, cote2);
//
//}



int test2(glm::mat4 model) {
	/*
	std::cout << "coordonnee x : " << model[3].x << std::endl;
	std::cout << "coordonnee y : " << model[3].y << std::endl;
	std::cout << "coordonnee z : " << model[3].z <<std::endl;
	*/

	Vertex triangle[] = {
	Vertex{glm::vec3(-1.0f, 0.f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)}
	};

	glm::vec2 origine2D = glm::vec2(2.f, 1.f);
	glm::vec2 dir2D = glm::vec2(-1.f, -1.f);

	rayCastTriangle(triangle, origine2D, dir2D);

	/*
	glm::vec3 origine = glm::vec3(-0.5f, -2.f, -0.5f);
	glm::vec3 dir = glm::vec3(0.f, 1.f, 0.f);
	origine = glm::vec3(model * glm::vec4(origine, 1.f));
	
	std::cout << "origine : " << glm::to_string(origine) << std::endl;
	std::cout << "direction : " << glm::to_string(dir) << std::endl;
	std::cout << glm::to_string(model * glm::vec4(triangle[0].position, 1.0)) << std::endl;
	std::cout << glm::to_string(model * glm::vec4(triangle[1].position, 1.0)) << std::endl;
	std::cout << glm::to_string(model * glm::vec4(triangle[2].position, 1.0)) << std::endl;
	*/
	//std::cout << glm::to_string(triIntersect(origine, glm::normalize(dir), triangle, model)) << std::endl;

	return 1;
}