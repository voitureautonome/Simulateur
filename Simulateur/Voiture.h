#pragma once
#include "Model.h"
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

class Voiture : public Model
{
	enum mode
	{
		lidar,
		vision,
		lidarETvision
	};
public:
	float vitesse;
	glm::vec3 forwardVector;
	float accelerationInput;
	float steerInput;
	Voiture();
	Voiture(float vitesse);
	void controleVoiture(GLFWwindow* window,double deltaTime);
};

