#pragma once
#include "Model.h"
#include <GLFW/glfw3.h>

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
	Voiture();
	Voiture(float vitesse);
	void controleVoiture(GLFWwindow* window,double deltaTime);
};

