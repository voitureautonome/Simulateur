#include "Voiture.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

Voiture::Voiture():Model::Model("models/car/car.gltf")
{
	this->forwardVector = glm::vec3(1.f);
	this->vitesse = 20.f;
	this->accelerationInput = 0.f;
	this->steerInput = 0.f;
}

Voiture::Voiture(float vitesse):Model::Model("models/car/car.gltf")
{
	this->vitesse = vitesse;
	this->forwardVector=glm::vec3(1.f);
	this->accelerationInput = 0.f;
	this->steerInput = 0.f;
}

void Voiture::controleVoiture(GLFWwindow* window,double deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		this->accelerationInput += 0.1f * (float) deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE) {
		this->accelerationInput -= 0.2f * (float)deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		this->position += glm::vec3(this->vitesse * deltaTime, 0.f, 0.f);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		this->position -= glm::vec3(this->vitesse * deltaTime, 0.f, 0.f);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		this->steerInput = -1.f;
		//this->rotation = glm::quat(glm::vec3(0.f, glm::radians(90.f), 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		this->steerInput = 1.f;
	}
	glm::vec3 accelerationVector = this->forwardVector * this->accelerationInput * 0.2f * (float) deltaTime;
	float steerAngle = steerInput * 2000.f* (float) deltaTime;
	glm::vec3 newForwardVector = glm::rotateY(this->forwardVector, glm::radians(steerAngle));
	this->forwardVector = glm::mix(this->forwardVector, newForwardVector, 0.1f * (float) deltaTime);
	this->model = glm::lookAt(forwardVector, forwardVector + this->position, glm::vec3(0.0f, 1.0f, 0.0f));
}