#include "Window.h"

Window::Window()
{
	m_width = 1280;
	m_height = 720;
}

Window::Window(int windowWidth, int windowHeigth)
{
	m_width = windowWidth;
	m_height = windowHeigth;
}

int Window::Init()
{
	//Inicializamos GLFW, superimportante para hacer cualquier cosa
	if (!glfwInit())
	{
		std::cout << "Error en inicialización de GLFW" << std::endl;
		glfwTerminate();
		return 1;
	}

	/// Crear propiedades de ventana de GLFW
	// Versión de OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Mayor versión que va a soportar
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Versión que debe tener nuestro procesador como mínimo para que nuestra aplicación funcione
	//Si detecta código en OpenGL desfasado, disparará errores
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Permitir compatibilidad hacia adelante
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//Creamos nuestra ventanita GLFW style
	m_mainWindow = glfwCreateWindow(m_width, m_height, "Test window", NULL, NULL);

	if (!m_mainWindow)
	{
		std::cout << "Error de creación de ventana de GLFW" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Conseguir la información del tamaño de buffer --> Conseguir el tamaño que hay en el área de la ventana (main framebuffer)
	glfwGetFramebufferSize(m_mainWindow, &m_bufferWidth, &m_bufferHeight);

	// Crear el contexto con el que GLEW funcionará (Dónde va a dibujar OpenGL)
	glfwMakeContextCurrent(m_mainWindow);

	// Permitir las extensiones modernas de OpenGL
	glewExperimental = GL_TRUE;

	//Inicializamos GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error de inicialización de GLEW" << std::endl;
		glfwDestroyWindow(m_mainWindow); //Nos cargamos la ventana porque ya habiamos creado una
		m_mainWindow = nullptr;
		glfwTerminate();
		return 1;
	}

	//Activamos el buffer de profundidad (Z-Buffer) --> http://docs.gl/gl3/glEnable
	glEnable(GL_DEPTH_TEST);

	//Especificar el tamaño del Viewport con los datos que nos ha dado GLFW --> http://docs.gl/gl3/glViewport
	//Le estamos diciendo a OpenGL qué área tiene para pintar
	glViewport(0, 0, m_bufferWidth, m_bufferHeight);
}


Window::~Window()
{
	//Destrucciones rudimentarias
	glfwDestroyWindow(m_mainWindow);
	glfwTerminate();
}
