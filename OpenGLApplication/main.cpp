#include<iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
const GLint WIDTH = 1280, HEIGHT = 720;

int main()
{
	if (!glfwInit())
	{
		std::cout << "Error initializing GLFW" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
	
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test window", NULL, NULL);

	if (!mainWindow)
	{
		std::cout << "Window failed to be created" << std::endl;
		glfwTerminate();
		return 1;
	}

	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferHeight, &bufferWidth);
	glfwMakeContextCurrent(mainWindow);

	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew initialization failed" << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glViewport(0,0, bufferWidth, bufferHeight);

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
	ImGui_ImplOpenGL3_Init("#version 150");
	ImGui::StyleColorsDark();

	glm::vec3 newMat = glm::vec3(0,0,0);
	std::cout << newMat.x << std::endl;

	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Hello world!");
		ImGui::End();

		ImGui::Render();
		
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(mainWindow);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

	return 0;
}