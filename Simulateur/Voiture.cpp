#include "Voiture.h"
#include <glm/gtx/string_cast.hpp>

Voiture::Voiture():Model::Model("models/car/car.gltf")
{
}

Voiture::Voiture(float vitesse):Model::Model("models/car/car.gltf")
{
	this->vitesse = vitesse;
}

void Voiture::controleVoiture(GLFWwindow* window,double deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		this->position += glm::vec3(this->vitesse * deltaTime, 0.f, 0.f);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		this->position -= glm::vec3(this->vitesse * deltaTime, 0.f, 0.f);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		this->position -= glm::vec3(this->vitesse * deltaTime, 0.f, 0.f);
		this->rotation = glm::quat(glm::vec3(0.f, glm::radians(90.f), 0.f));
	}
}
