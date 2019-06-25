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
#include "Light.h"

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

#pragma region AverageNormals
void calculateAverageNormals(unsigned int* indices, unsigned int indiceCount, float* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normaloffset)
{
	//Para poder hacer un shading de Phong, hemos de hacer una media de las normales que tienen los vértices en una superficie
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		/*
			Es la forma de extraer todos los vértices que queremos evitando las coordenadas de textura y normales
		*/
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		//Cogemos los vectores resultantes de la diferencia entre el vértice que tenemos escogido y los que forman con él la figura
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);

		//Hacemos el producto vectorial de estos vectores y obtenemos la normal de la superficie
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normaloffset; in1 += normaloffset; in2 += normaloffset; //Ahora tiramos para la posición en la que se encuentra la normal de nuestro vértice
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	//Ahora vamos a normalizar los vectores normales resultantes
	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normaloffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}
#pragma endregion

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
			-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
		};

	calculateAverageNormals(indices, 12, vertices, 32, 8, 5);

	mesh = new Mesh();
	mesh->CreateMesh(vertices, indices, 32, 12);
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

	Light* light = new Light(shader->GetLightData());

	//fov estandar = 45.0f
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);

	unsigned int uniformProjection = 0, uniformModel = 0;
	// Hacer un bucle hasta que la ventana se cierre

	GUI ImGUIInterface = GUI(mainWindow.GetWindowptr());

	ImGUIInterface.SetLightToHandle(light);

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
		shader->SetCameraPosition(camera.GetCameraPosition());

		light->RenderLight();

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

	delete light;
	light = nullptr;

	DeletePointers();
	return 0;
}