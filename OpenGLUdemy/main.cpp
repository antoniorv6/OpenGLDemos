#include <iostream>
#include <cmath>
#include <vector>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

Mesh* mesh;
Shader* shader;

Window mainWindow;

float curAngle = 0.0f;

//Vertex Shader Location
static const char* vShader = "Shaders/shader.vert";

//Fragment Shader Location
static const char* fShader = "Shaders/shader.frag";

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	float vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	mesh = new Mesh();
	mesh->CreateMesh(vertices, indices, 12, 12);
}

void CreateShaders()
{
	shader = new Shader();
	shader->CreateFromFiles(vShader, fShader);
}

void MakeTransformations(glm::mat4 &model)
{
	///Modelo de Puchol
	//glm::translate lo que hace es aplicar una operación de traslación sobre la matriz modelo, es decir, hemos acumulado una traslación
	//Traslacion (matrizmodelo, vector de desplazamiento)
	//model = glm::translate(model, glm::vec3(0.6f, 0.1f, -1.5f));
	//Rotación (matrizmodelo, grados, eje);
	//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
	
	///Modelo normal
	glm::mat4 unity = glm::mat4(1);

	glm::mat4 translation = glm::translate(unity, glm::vec3(0.6f, 0.1f, -1.5f));
	glm::mat4 rotation = glm::rotate(unity, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//Escalado OJO CON ESTE (matrizmodelo, vector de escalado [cuidado, si no queremos escalar hay que multiplicar por 1.0f])
	glm::mat4 scale = glm::scale(unity, glm::vec3(0.4f, 0.4f, 1.0f));
	model = rotation * translation;

}

void DeletePointers()
{
	delete mesh;
	mesh = nullptr;
	
	delete shader;
	shader = nullptr;
}

int main()
{
	mainWindow = Window(1280,720);
	mainWindow.Init();

	CreateObjects();
	CreateShaders();

	//fov estandar = 45.0f
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	unsigned int uniformProjection = 0, uniformModel = 0;
	// Hacer un bucle hasta que la ventana se cierre
	while (!mainWindow.getShouldClose())
	{
		//Conseguir y manejar los inputs de usuario
		glfwPollEvents();

		/*Poner color a la ventana y limpiarla --> http://docs.gl/gl3/glClearColor
		http://docs.gl/gl3/glClear */
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Limpia tanto el buffer de color como el de profundidad

		//Cogemos el programa que hemos inicializado --> http://docs.gl/gl3/glUseProgram
		shader->UseShader();

		glm::mat4 model; //Todos los valores de esta matriz son 0 excepto la diagonal principal, sirve para acumular transformaciones
		
		MakeTransformations(model);
		
		shader->SetMatrixes(projection, model);

		mesh->RenderMesh();
		
		//Quitamos el programa
		glUseProgram(0);

		//Cogemos el buffer en el que hemos pintado el color rojo y lo ponemos en la pantalla
		//Al principio este buffer es invisible
		mainWindow.swapBuffers();
	}

	DeletePointers();
	return 0;
}