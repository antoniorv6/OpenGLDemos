# OpenGLDemos
### Demos simples de OpenGL

## Parte 1 - OpenGL básico: Inicialización, shader básico, creación de una malla simple y matrices de transformación

Esta sección es la más engorrosa de todas, puesto que se trata de la inicialización de OpenGL y todas sus librerías para el correcto funcionamiento de la aplicación.

En este documento intentaré repasar por encima los procesos que se siguen para llegar al ejemplo y dar algún apunte que puede resultar interesante.

:warning: __*No es necesario descargar nada adicional, aparte de poseer una versión de Microsoft Visual Studio posterior a la 2015.*__

#### Inicialización de OpenGL

La inicialización de OpenGL es medianamente sencilla. Aquí se ha abstraido prácticamente a la clase Window.cpp

La función más importante dentro de esta clase es Init:

```c++
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
```
Gracias a este código, ya tendríamos inicializado tanto GLFW como GLEW para que nuestra aplicación de OpenGL funcionase correctamente.

**¿Qué es GLEW y qué es GLFW?**

GLEW es una librería que nos permite utilizar las funciones de OpenGL que vienen dentro de nuestro sistema, dependientes del procesador y sistema operativo que tengamos, al igual que la tarjeta gráfica.

GLFW es una librería que nos permite crear y gestionar eventos de ventana para dar interacción al usuario con la misma.

#### Shaders básicos
Dado que en las próximas secciones ahondaremos en el tema relacionado a los shaders, cabe destacar solo unos detalles a tener en cuenta.

* Los shaders también se compilan y linkan como cualquier programa en C++, pero OpenGL tiene un funcionamiento específico
* Hay que pasarlos sí o sí como ```const char *``` para que esto funcione
* Hemos de controlar todo tipo de errores. Puede haber tres tipos
  * Errores de compilación
  * Errores de linkado
  * Errores de validación

Hay algunas definiciones que cabe destacar, ya que se usan en esta demo:

**Atributos**: En el código del Shader aparecen como, por ejemplo, ``` layout(location = 0) in vec3 pos ```. **Son conjuntos de datos asociados al vértice en concreto**. Durante la carga de la malla comprenderemos un poco mejor este concepto.

**Variables uniform**: Son variables que podemos enviar desde nuestro programa al shader. Es un método de comunicación entre el procesador y la tarjeta gráfica.

#### Carga de una malla poligonal simple

Todo el proceso se ha intentado detallar en la clase Mesh.cpp
Los conceptos básicos con los que nos hemos de quedar en esta demo son los siguientes:

* __VAO__: *Vertex Attribute Object* -- Este objeto nos permite especificar cómo vamos a mandar un cierto atributo en nuestra malla poligonal. Es el concepto más importante, puesto que sirve a OpenGL como un manual de instrucciones a la hora de identificar los atributos de los vértices. En el código está explicado con cierto detalle. A nivel teórico debemos quedarnos con que es una especificación que nosotros hacemos al Vertex Shader para indicarle cómo debe identificar los atributos de los vértices.
* __IBO__: *Index Buffer Object* -- Objeto que nos permite indicarle a OpenGL en qué orden debe unir los vértices de nuestra geometría. A nivel práctico sería como enviarle un anagrama para que éste lo autocomplete.
* __VBO__: *Vertex Buffer Object* -- Objeto que contiene todos los datos de los vértices de la malla poligonal.

Ahondaremos más en los conceptos que envuelven la carga de la malla cuando carguemos mallas poligonales complejas

#### Matrices de transformación
En esta demo, hay una interfaz de usuario que permite modificar las variables de transformación de nuestra geometría al gusto de consumidor.
Lo más importante a tener en cuenta es el **orden de las operaciones** ya que al tratarse de multiplicaciones de matrices,  el resultado obtenido nunca será el mismo si se altera el orden.
Dentro de la función ```MakeTransformations()``` están las operaciones que se realizan para obtener el resultado. Hay dos formas de aproximarse a este problema, ambas especificadas en el código.

Mi consejo: ¡Trastea con ella! Lo mejor es observar los efectos que se producen al cambiar el orden de las multiplicaciones para entender por qué es importante el concepto.
(Pista): Cuando haces una Traslación, el pivote de la malla poligonal no se desplaza, pero el objeto en cuestión sí.

Existe una matríz adicional: la matriz de Proyección (la cual veremos en otro ejemplo más en profundidad)

Como se puede observar, el resultado de estas matrices se envía al Vertex Shader a través de un **uniform**. Para operar con uniforms, hemos de pedirle un identificador a OpenGL para, después, enviarle el valor de nuestras matrices.
El proceso se describiría con este código (Aunque en el código real hay una optimización para no estar pidiendo constantemente el mismo identificador al Shader)
```c++
m_uniformMVP = glGetUniformLocation(m_shaderID, "MVP");
///Esto es una optimización a nivel de CPU
//Multiplicamos la Proyección por la Model
//ESTA MULTIPLICACIÓN SIEMPRE ES ASÍ
glm::mat4 result = c_projection * c_model;
	
//Le decimos al shader que le vamos a pasar una matriz de 4*4 con floats a un uniform que tiene el ID que indicamos
//con m_uniformMVP
glUniformMatrix4fv(m_uniformMVP, 1, GL_FALSE, glm::value_ptr(result));
```
