#pragma once

#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
class Window
{
public:
	Window();
	Window(int windowWidth, int windowHeight);

	int Init();

	float GetBufferWidth()	{ return m_bufferWidth; }
	float GetBufferHeight()	{ return m_bufferHeight; }

	bool GetShouldClose()	{ return glfwWindowShouldClose(m_mainWindow); }

	void SwapBuffers()		{ glfwSwapBuffers(m_mainWindow); }

	void GetGLFWEvents() { glfwPollEvents();}

	bool* GetKeys() { return m_keys; }

	GLFWwindow* GetWindowptr() { return m_mainWindow; }


	~Window();

private:
	GLFWwindow * m_mainWindow;

	int m_width, m_height;				//Tamaño de ventana
	int m_bufferWidth, m_bufferHeight;	//Tamaño del buffer de renderizado (suele ser el de la ventana, pero tampoco os confiéis porque en realidad son dos cosas distintas)

	bool m_keys[1024];

	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
};

