#include "raycast.h"

float intersection(float a1, float b1, float a2, float b2) {
	// On cherche à trouver où les droites se croisent donc à résoudre a1x + b1 = a2x + b2 
	// on pose l'équation (a1x + b1) - (a2x + b2) = 0
	// on factorise par x :  x(a1 - a2) + (b1 - b2) = 0
	// on trouve x = (b2 - b1) / (a1 - a2)

	return (b2 - b1) / (a1 - a2);
}

bool rayCast(glm::vec2 wp1, glm::vec2 wp2, glm::vec2 rayStart, glm::vec2 rayDirection, glm::vec2& res) {
	const float x1 = wp1.x;
	const float y1 = wp1.y;
	const float x2 = wp2.x;
	const float y2 = wp2.y;

	const float x3 = rayStart.x;
	const float y3 = rayStart.y;
	const float x4 = rayStart.x + rayDirection.x;
	const float y4 = rayStart.y + rayDirection.y;

	// equation de droite ax + b

	// coefficient directeur de la droite représentant le mur
	float a1 = (y2 - y1) / (x2 - x1);
	// coefficient directeur de la droite représentant le rayon
	float a2 = (y4 - y3) / (x4 - x3);

	// Si les deux droites ont le même coeff directeur, elles sont parallèles
	if (a1 == a2)
	{
		return false;
	}

	// Sinon on calcule leur ordonné à l'origine
	float b1 = y1 - a1 * x1;
	float b2 = y3 - a2 * x3;

	// On a alors deux droites a1x + b1 et a2x + b2
	// On cherche pour quelle valeur de X elles se croisent
	float resX = intersection(a1, b1, a2, b2);

	// On remplace dans l'équation d'une des deux droites les coordonnées
	float resY = a1 * resX + b1;

	// On crée un vecteur avec les coordonnées de l'intersection, et on le met dans le résultat
	res = glm::vec2(resX, resY);
	return true;
}

float distance(glm::vec2 A, glm::vec2 B) {
	return sqrt(pow(B.x - A.x, 2) + pow(B.y - A.y, 2));
}

float rayCastTriangle(triangle tri, glm::vec2 rayStart, glm::vec2 rayDirection) {
	
	// On récupère 3 points en 2D, correspondant aux angles du triangle
	glm::vec2 pointA = glm::vec2(tri.angle[0].x, tri.angle[0].z);
	glm::vec2 pointB = glm::vec2(tri.angle[1].x, tri.angle[1].z);
	glm::vec2 pointC = glm::vec2(tri.angle[2].x, tri.angle[2].z);

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
	//std::cout << "resultat : " << glm::to_string(res) << std::endl;
	// std::cout << "longueur : " << longueur << std::endl;
	return longueur;

}


void simuLidar(Model laby, Model voit) {

	// Parcours de chaque mesh du modèle
	for (size_t i = 0; i < laby.meshes.size(); i++)
	{

		// On crée un tableau de triangles les stocker au fur et à mesure
		std::vector<triangle> vecTri;

		for (size_t j = 0; j < laby.meshes[i].vertices.size(); j+=3)
		{
			// On vérifie qu'il y au moins 3 angles pour former un triangle
			if (j+2 < laby.meshes[i].vertices.size())
			{

				// On récupère les indices de chaque angle du triangle
				int indice1 = laby.meshes[i].indices[j];
				int indice2 = laby.meshes[i].indices[j+1];
				int indice3 = laby.meshes[i].indices[j+2];
				// On crée un triangle en lui passant les coordonnées de ses 3 angles
				triangle tri;

				tri.angle[0] = laby.meshes[i].vertices[indice1].position;
				tri.angle[1] = laby.meshes[i].vertices[indice2].position;
				tri.angle[2] = laby.meshes[i].vertices[indice3].position;
				
				//std::cout << "TRIANGLE NUM " << numTri << std::endl;
				//std::cout << "angle 1 " << glm::to_string(laby.meshes[i].vertices[indice1].position) << std::endl;
				//std::cout << "angle 2 " << glm::to_string(laby.meshes[i].vertices[indice2].position) << std::endl;
				//std::cout << "angle 3 " << glm::to_string(laby.meshes[i].vertices[indice3].position) << std::endl;

				// On ajoute les triangles à notre tableau
				vecTri.push_back(tri);
			}
		}

		glm::vec3 posOrigine = voit.position;

		glm::vec2 origine2D = glm::vec2(posOrigine.x, posOrigine.z);
		glm::vec2 dir2D = glm::normalize(glm::vec2(1.f, 0.f));

		double min = 1e+07;

		for (size_t i = 0; i < vecTri.size(); i++)
		{
			float longueur = rayCastTriangle(vecTri[i], origine2D, dir2D);
			std::cout << "==========================================================" << std::endl;
			std::cout << "  Triangle " << i << " : " << longueur << std::endl;
			std::cout << "  Angles: " << glm::to_string(vecTri[i].angle[0]) << " " << glm::to_string(vecTri[i].angle[1]) << " " << glm::to_string(vecTri[i].angle[2]) << std::endl;

			if (longueur < min)
				min = longueur;
		}
		std::cout << "==========================================================" << std::endl;

		std::cout << "  Voiture " << i << " : " << glm::to_string(voit.position) << " direction " << glm::to_string(dir2D) << std::endl;

		std::cout << "longueur minimum: " << min << std::endl;
		
	}

	/*
	for (size_t i = 0; i < laby.meshes[0].indices.size(); i++)
	{
		std::cout << "INDICE " << i << " || VALEUR " << laby.meshes[0].indices[i] << std::endl;

	}
	std::cout << glm::to_string(laby.meshes[0].vertices[35].position) << std::endl;
	std::cout << glm::to_string(laby.meshes[0].vertices[38].position) << std::endl;
	std::cout << glm::to_string(laby.meshes[0].vertices[11].position) << std::endl;
	*/
}


int testRaycast() {
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

	float a1 = 1;
	float b1 = 0;
	float a2 = -1;
	float b2 = 6;

	std::cout << " Intersection " << intersection(a1, b1, a2, b2) << std::endl;

	glm::vec2 a = glm::vec2(2, 2);
	glm::vec2 b = glm::vec2(4, 4);
	glm::vec2 c = glm::vec2(2, 4);
	glm::vec2 direction = glm::normalize(glm::vec2(1, -1));

	glm::vec2 res;

	std::cout << "Intersection ? " << rayCast(a, b, c, direction, res) << std::endl;
	if (rayCast(a, b, c, direction, res))
	{
		std::cout << "Ou ? " << glm::to_string(res) << std::endl;
	}

	//rayCastTriangle(triangle, origine2D, dir2D);

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