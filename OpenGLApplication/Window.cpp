#include "Window.h"
#include <iostream>

Window::Window(GLuint c_width, GLuint c_height)
{
	m_width = c_width;
	m_height = c_height;

	m_bufferHeight = 0;
	m_bufferWidth = 0;

	Init();
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Window::Init()
{
	if (!glfwInit())
	{
		std::cout << "Error initializing window" << std::endl;
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

	//Creamos la ventanita
	m_window = glfwCreateWindow(m_width, m_height, "PruebecicasOpenGL", NULL, NULL);

	if (!m_window)
	{
		std::cout << "Window failed to be created" << std::endl;
		glfwTerminate();
		return;
	}

	//A partir de aquí ya tenemos nuestra ventanita
	//Debemos crear el contexto en el que OpenGL puede renderizar el framBuffer principal

	glfwGetFramebufferSize(m_window, &m_bufferWidth, &m_bufferHeight);
	glfwMakeContextCurrent(m_window);

	//Ventana terminada!!

	InitGlew();
}

void Window::InitGlew()
{
	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error initializing GLEW" << std::endl;
		return;
	}

	//Como ya tenemos el contexto de antes, ahora lo que hay que hacer es decirle a OpenGL que contexto es donde ha de dibujar
	glViewport(0, 0, m_bufferHeight, m_bufferWidth);
}

void Window::ClearWindow()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(m_window);
}
