#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "MenuConnexion.h"
#include "menuVoiture.h"
#include "Line.h"
#include "glm/vec2.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void window_size_callback(GLFWwindow* window, int width, int height)
{
	if (height < 100)
		height = 100;
	glfwSetWindowSize(window,width, (int)(width/1.25f));
	glViewport(0, 0, width, (int)(width / 1.25f));
}

void HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void ShowConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

bool IsConsoleVisible()
{
	return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

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
// Vertices coordinates
GLfloat vertices[] =
{
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
	-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
	0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
	0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
};
// Indices for vertices order
GLuint indices[] =
{
	0, 3, 5, // Lower left triangle
	3, 2, 4, // Lower right triangle
	5, 4, 1, // Upper triangle
};

GLuint linesIndices[] =
{
	0,1,2,3,4,5
};


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

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(800, 800, "Simulateur Auto-mobile", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	

	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);

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
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	Shader shaderLigne("ligne.vert","ligne.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO
	VAO1.LinkVBO(VBO1, 0);
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	VAO VAO2;
	VAO2.Bind();
	VBO VBO2(&Line::listeLignes.data()[0], Line::listeLignes.size()*sizeof(GLfloat));
	EBO EBO2(&Line::listeIndices.data()[0], Line::number * 2 * sizeof(GLuint));
	
	VAO2.LinkVBO(VBO2, 0);
	VAO2.Unbind();
	VBO2.Unbind();
	EBO2.Unbind();
	//HideConsole(); // c'est un GUI on cache la console si tout charge bien


	//gestion du temps
	const double updateInterval = 1.0 / 60.0;
	double lastTime = 0.0;
	bool show_demo_window = true;
	// Main while loop
	static float f = 0.0f;
	bool showProfiler = false;
	bool show_MenuConnexion = false;

	//io.Fonts->AddFontDefault();
	ImFontConfig config;
	config.OversampleH = 2;
	config.OversampleV = 1;
	config.GlyphExtraSpacing.x = 1.0f;
	io.Fonts->AddFontFromFileTTF("Roboto-Regular.ttf", 18.0f,&config); 


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		double time = glfwGetTime();
		double deltaTime = time - lastTime;
		if (deltaTime < updateInterval) {
			continue;
		}
		lastTime = time;

		
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		shaderProgram.Activate();
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
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
				if (ImGui::MenuItem("showProfiler")) {
					showProfiler = !showProfiler;
					std::cout << "Menu connexion " << showProfiler << std::endl;
				}
				ImGui::EndMenu();
			}
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
			handleMenuVoiture(&show_MenuConnexion);
			
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
		glClear(GL_COLOR_BUFFER_BIT);
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}