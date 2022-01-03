#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#ifndef NOMINMAX
# define NOMINMAX
#endif
#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "MenuConnexion.h"
#include "menuVoiture.h"
#include "Line.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"
#include "Camera.h"
#include "windowUtils.h"
#include "Mesh.h"
void tournerVecteur(glm::vec2 *v, float angle) {
	float x2 = v->x * cos(angle) - v->y * sin(angle);
	float y2 = v->x * sin(angle) + v->y * cos(angle);
	v->x = x2;
	v->y = y2;
}
glm::vec2 rayCast(glm::vec2 wp1,glm::vec2 wp2,glm::vec2 rayStart,glm::vec2 rayDirection) {
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
		return glm::vec2(-2.f,-2.f);
	}

	const float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
	const float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
	if (t > 0 && t < 1 && u > 0) {
		return glm::vec2(x1 + t * (x2 - x1), y1 + t * (y2 - y1));
	}
	else {
		return glm::vec2(-2.f, -2.f);
	}
}
glm::vec3 triIntersect(glm::vec3 ro,glm::vec3 rd,glm::vec3 v0,glm::vec3 v1,glm::vec3 v2)
{
	glm::vec3 v1v0 = v1 - v0;
	glm::vec3 v2v0 = v2 - v0;
	glm::vec3 rov0 = ro - v0;
	glm::vec3  n = glm::cross(v1v0, v2v0); // cross produit vectoriel
	glm::vec3  q = glm::cross(rov0, rd);
	float d = 1.0f / dot(rd, n); // dot produit scalaire
	float u = d * dot(-q, v2v0);
	float v = d * dot(q, v1v0);
	float t = d * dot(-n, rov0);
	if (u < 0.0f || u>1.0f || v < 0.0f || (u + v)>1.0f)
		t = -1.0f;
	return glm::vec3(t, u, v);
}

Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           TexCoord         /       NORMALS         //
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

GLuint linesIndices[] =
{
	0,1,2,3,4,5
};
const unsigned int width = 800;
const unsigned int height = 800;

int main()
{
	Line::initLines();
	Line::drawLine(glm::vec2(0.0f, 0.0f), glm::vec2(1.f, 0.0f), ImVec4(1.f, 2.f, 3.f,1.f));
	Line::drawLine(glm::vec2(0.0f, 0.0f), glm::vec2(0.f, 1.0f), ImVec4(1.f, 2.f, 3.f, 1.f));
	Line::drawLine(glm::vec2(0.f, 0.f), glm::vec2(-0.2f, 0.5f), ImVec4(1.f, 2.f, 3.f, 1.f));
	Line::drawLine(glm::vec2(1.f, 0.5f), glm::vec2(-0.2f, 0.5f), ImVec4(1.f, 2.f, 3.f, 1.f)); //up

	//Line::drawLine(glm::vec2(0.f, 0.0f), glm::vec2(0.0f, -0.9f), ImVec4(1.f, 2.f, 3.f, 1.f));
	Line::drawLine(glm::vec2(0.99f, 0.0f), glm::vec2(0.99f, -0.9f), ImVec4(1.f, 2.f, 3.f, 1.f));
	Line::drawLine(glm::vec2(0.0f, -0.9f), glm::vec2(1.f, -0.9f), ImVec4(1.f, 2.f, 3.f, 1.f));
	glm::vec2 dir = glm::vec2(1.f, 1.f);
	tournerVecteur(&dir, 0.1f);
	glm::vec2 res = rayCast(glm::vec2(1.f,0.5f), glm::vec2(-0.2f,0.5f), glm::vec2(0.f,0.f), dir);
	Line::drawLine(glm::vec2(0.f, 0.f), res, ImVec4(1.f, 1.f, 1.f, 1.f));

	//Line::drawLine(glm::vec2(0.0f, 0.5f), glm::vec2(0.5, 0.0f), ImVec4(1.f, 2.f, 3.f, 1.f));

	// Initialize GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);


	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
	GLFWwindow* window = glfwCreateWindow(width, height, "Simulateur Auto-mobile", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	GLFWimage images[1];
	images[0].pixels = stbi_load("charly.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	const int taux_rafraichissement = glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	// Setup Platform/Renderer bindings

	// Setup Dear ImGui style

	const char* glsl_version = "#version 330";
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	glViewport(0, 0, width, height);

	Texture textures[]
	{
		Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	Shader shaderProgram("default.vert", "default.frag");
	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	Mesh floor(verts, ind, tex);

	Shader lightShader("light.vert", "light.frag");
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh light(lightVerts, lightInd, tex);

	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 1.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);


	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform3f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Shader shaderLigne("ligne.vert", "ligne.frag");
	VAO VAO2;
	VAO2.Bind();
	VBO VBO2(&Line::listeLignes.data()[0], Line::listeLignes.size()*sizeof(GLfloat));
	EBO EBO2(&Line::listeIndices.data()[0], ((unsigned long long) Line::number) * 2 * sizeof(GLuint));
	
	VAO2.LinkVBO(VBO2, 0);
	VAO2.Unbind();
	VBO2.Unbind();
	EBO2.Unbind();
	//HideConsole(); // c'est un GUI on cache la console si tout charge bien


	//gestion du temps
	const double updateInterval = 1.0 / taux_rafraichissement;
	double lastTime = 0.0;
	bool show_demo_window = true;

	static float f = 0.0f;
	bool showProfiler = false;
	bool show_MenuConnexion = false;
	bool show_MenuSimulation = true;

	//io.Fonts->AddFontDefault();
	ImFontConfig config;
	config.OversampleH = 2;
	config.OversampleV = 1;
	config.GlyphExtraSpacing.x = 1.0f;
	io.Fonts->AddFontFromFileTTF("Roboto-Regular.ttf", 18.0f,&config); 

	float rotation = 0.0f;

	glEnable(GL_DEPTH_TEST);
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		//ImGui::IsMouseHoveringRect(ImVec2(0, 0), ImVec2(ImGui::GetIO().DisplaySize.x, 24));

		double time = glfwGetTime();
		double deltaTime = time - lastTime;
		if (deltaTime < updateInterval) {
			continue;
		}
		lastTime = time;
		rotation += (float) 1.f * deltaTime;
		//lightModel = glm::rotate(lightModel, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		//lightModel = glm::translate(lightModel, glm::vec3(0, 0.1*deltaTime, 0.0f));
		lightShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));

		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		shaderProgram.Activate();
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightModel[3].x, lightModel[3].y, lightModel[3].z);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		floor.Draw(shaderProgram,camera);
		light.Draw(lightShader,camera);
		
		
		//VAO1.Unbind();
		VAO2.Bind();
		shaderLigne.Activate();
		glDrawElements(GL_LINES, Line::number*2, GL_UNSIGNED_INT,0);
		VAO2.Unbind();
		//Line::drawLine(glm::vec2(0.4f, 2.0f), glm::vec2(0.1f, 0.05f), ImVec4(1.f, 2.f, 3.f, 1.f));


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Fichier"))
			{
				if (ImGui::MenuItem("Connexion"))
				{
					show_MenuConnexion = !show_MenuConnexion;
				}
				if (ImGui::MenuItem("Quitter")) {
					exit(0);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Options")) {
				if (ImGui::MenuItem("Profiler")) {
					showProfiler = !showProfiler;
					std::cout << "Menu connexion " << showProfiler << std::endl;
				}
				if (ImGui::MenuItem("Console")) {
					if (IsConsoleVisible())
						HideConsole();
					else
						ShowConsole();
				}
				ImGui::EndMenu();
			}
			ImGui::Text("FPS %.1f",ImGui::GetIO().Framerate);
			ImGui::EndMainMenuBar();
		}

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		{
			static int counter = 0;
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			ImGui::SetNextWindowPos(ImVec2((float)width, (float)height), ImGuiCond_FirstUseEver);
			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("taille de la fenetre width : %d height : %d ", (float)width, (float)height);               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
			/*ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/
		}
		{
			/*ImGui::NewFrame();

			int width, height;
			glfwGetWindowSize(window, &width, &height);
			ImGui::SetNextWindowPos(glm::vec2(50, 300), ImGuiCond_FirstUseEver);*/
			handleMenuConnexion(&show_MenuConnexion);
			handleMenuVoiture(&show_MenuSimulation);
			
		}
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
		// Specify the color of the background
		glClearColor(((float*)&clear_color)[0], ((float*)&clear_color)[1], ((float*)&clear_color)[2], 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	shaderProgram.Delete();
	lightShader.Delete();
	shaderLigne.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}