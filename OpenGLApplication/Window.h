#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window(GLuint c_width, GLuint c_height);
	~Window();
	void Init();
	void InitGlew();
	bool WindowHasToClose() { return glfwWindowShouldClose(m_window); };
	void GetGLFWEvents() { glfwPollEvents(); };
	void ClearWindow();
	void SwapBuffers();

private:
	GLFWwindow* m_window;
	GLuint m_width, m_height;
	int m_bufferWidth, m_bufferHeight;
};

