#include <iostream>
#include <cmath>
#include <vector>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "GUI.h"
#include "Camera.h"

Mesh* mesh;
Mesh* mesh2;
Shader* shader;

Window mainWindow;
Camera camera;

#pragma region GUIVariables
float xTranslate = 0.0f;
float yTranslate = 0.0f;
float zTranslate = -2.5f;
float rotationAngle = 0.0f;

bool xRotation = true;
bool yRotation = false;
bool zRotation = false;

float xScale = .4f;
float yScale = .4f;
float zScale = .4f;
#pragma endregion

//Vertex Shader Location
static const char* vShader = "Shaders/shader.vert";

//Fragment Shader Location
static const char* fShader = "Shaders/shader.frag";

static const char* textureLocation = "Texture/brick.jpg";

glm::mat4 model; //Matriz de transformación de nuestra malla, gracias a ella se mueve!!!!

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};
	
	float vertices[] = {
		//	 x      y     z			u     v
			-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,
			1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f
		};

	mesh = new Mesh();
	mesh->CreateMesh(vertices, indices, 20, 12);
	mesh->SetTexture(textureLocation);

	mesh2 = new Mesh();
	mesh2->CreateMesh(vertices, indices, 12, 12);
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

	glm::mat4 translation = glm::translate(unity, glm::vec3(xTranslate, yTranslate, zTranslate));
	glm::mat4 rotation = glm::rotate(unity, rotationAngle, glm::vec3(xRotation, yRotation, zRotation));
	//Escalado OJO CON ESTE (matrizmodelo, vector de escalado [cuidado, si no queremos escalar hay que multiplicar por 1.0f])
	glm::mat4 scale = glm::scale(unity, glm::vec3(xScale, yScale, zScale));

	(!xRotation && !yRotation && !zRotation) ? rotation = glm::mat4(1) : rotation;

	model = translation * rotation * scale;

}

void DeletePointers()
{
	delete mesh;
	mesh = nullptr;

	delete mesh2;
	mesh2 = nullptr;
	
	delete shader;
	shader = nullptr;
}

int main()
{
	mainWindow = Window(1280,720);
	mainWindow.Init();

	CreateObjects();
	CreateShaders();

	camera = Camera();

	//fov estandar = 45.0f
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);

	unsigned int uniformProjection = 0, uniformModel = 0;
	// Hacer un bucle hasta que la ventana se cierre

	GUI ImGUIInterface = GUI(mainWindow.GetWindowptr());

	while (!mainWindow.GetShouldClose())
	{
		//Conseguir y manejar los inputs de usuario
		glfwPollEvents();

		/*Poner color a la ventana y limpiarla --> http://docs.gl/gl3/glClearColor
		http://docs.gl/gl3/glClear */
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Limpia tanto el buffer de color como el de profundidad

		ImGUIInterface.SetGUI();

		camera.HandleKeys(mainWindow.GetKeys());
		camera.UpdateCamera();

		///PROCESO DE RENDERIZADO
		//Le decimos a OpenGL que vamos a usar estos shaders en específico --> http://docs.gl/gl3/glUseProgram
		shader->UseShader();

		ImGUIInterface.SetVariables(xTranslate, yTranslate, zTranslate, rotationAngle, xRotation, yRotation, zRotation, xScale, yScale, zScale);

		MakeTransformations(model);

		shader->SetMatrixes(projection, model, camera.GetViewMatrix());

		mesh->RenderMesh();

		//glm::mat4 unity = glm::mat4(1);
		//glm::mat4 translation2 = glm::translate(unity, glm::vec3(1.0f, 0.0f, -3.0f));
		//shader->SetMatrixes(projection,translation2, camera.GetViewMatrix());
		//mesh2->RenderMesh();
		
		//Quitamos el programa
		glUseProgram(0);

		ImGUIInterface.RenderGUI();

		///FIN DE PROCESO DE RENDERIZADO

		//Cogemos el buffer en el que hemos pintado el color rojo y lo ponemos en la pantalla
		//Al principio este buffer es invisible
		mainWindow.SwapBuffers();
	}

	DeletePointers();
	return 0;
}