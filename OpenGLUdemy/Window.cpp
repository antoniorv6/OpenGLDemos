#include "Window.h"

Window::Window()
{
	width = 1280;
	height = 720;
}

Window::Window(int windowWidth, int windowHeigth)
{
	width = windowWidth;
	height = windowHeigth;
}

int Window::Init()
{
	if (!glfwInit())
	{
		std::cout << "Error en inicialización de GLFW" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Crear propiedades de ventana de GLFW
	// Versión de OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Si detecta código en OpenGL desfasado, disparará errores
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Permitir compatibilidad hacia adelante
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Test window", NULL, NULL);

	if (!mainWindow)
	{
		std::cout << "Error de creación de ventana de GLFW" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Conseguir la información del tamaño de buffer --> Conseguir el tamaño que hay en el área de la ventana (buffer)
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Crear el contexto con el que GLEW funcionará (Dónde va a dibujar OpenGL)
	glfwMakeContextCurrent(mainWindow);

	// Permitir las extensiones modernas de OpenGL
	glewExperimental = GL_TRUE;

	//Inicializamos GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error de inicialización de GLEW" << std::endl;
		glfwDestroyWindow(mainWindow); //Nos cargamos la ventana porque ya habiamos creado una
		mainWindow = nullptr;
		glfwTerminate();
		return 1;
	}

	//Activamos el buffer de profundidad (Z-Buffer) --> http://docs.gl/gl3/glEnable
	glEnable(GL_DEPTH_TEST);

	//Especificar el tamaño del Viewport --> http://docs.gl/gl3/glViewport
	glViewport(0, 0, bufferWidth, bufferHeight);
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
