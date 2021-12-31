#ifndef WINDOW_UTILS
#define WINDOW_UTILS

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Windows.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void window_size_callback(GLFWwindow* window, int width, int height);
void HideConsole();
void ShowConsole();
bool IsConsoleVisible();
void processInput(GLFWwindow* window);

#endif // !WINDOW_UTILS