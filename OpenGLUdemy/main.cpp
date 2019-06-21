#include <iostream>
#include <cmath>
#include <vector>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

#include "IMGUIuse/imgui.h"
#include "IMGUIuse/imgui_impl_opengl3.h"
#include "IMGUIuse/imgui_impl_glfw.h"

Mesh* mesh;
Shader* shader;

Window mainWindow;

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
	
	delete shader;
	shader = nullptr;
}

void ResetImGUI()
{
	xTranslate = 0.0f;
	yTranslate = 0.0f;
	zTranslate = -2.5f;
	rotationAngle = 0.0f;

	xRotation = true;
	yRotation = false;
	zRotation = false;

	xScale = .4f;
	yScale = .4f;
	zScale = .4f;
}

#pragma region IMGUIImpl
void InitIMGUI()
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(mainWindow.GetWindowptr(), true);
	ImGui_ImplOpenGL3_Init("#version 150");
}

void CreateIMGUIFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void SetImGuiInterface()
{
	ImGui::SetNextWindowSize(ImVec2(300, 400));
	ImGui::Begin("Controles increibles");
	ImGui::Text("Translacion");
	ImGui::SliderFloat(" X", &xTranslate, -5.0f, 5.0f);
	ImGui::SliderFloat(" Y", &yTranslate, -5.0f, 5.0f);
	ImGui::SliderFloat(" Z", &zTranslate, -5.0f, 5.0f);

	ImGui::Separator();
	ImGui::Text("Rotacion");
	ImGui::SliderAngle(" Angulo", &rotationAngle, -360.0f, 360.0f);
	ImGui::Checkbox("Eje X", &xRotation);
	ImGui::Checkbox("Eje Y", &yRotation);
	ImGui::Checkbox("Eje Z", &zRotation);

	ImGui::Separator();
	ImGui::Text("Escalado");
	ImGui::SliderFloat(" XS", &xScale, -5.0f, 5.0f);
	ImGui::SliderFloat(" YS", &yScale, -5.0f, 5.0f);
	ImGui::SliderFloat(" ZS", &zScale, -5.0f, 5.0f);

	if (ImGui::Button("Reset", ImVec2(50, 30)))
	{
		ResetImGUI();
	}

	ImGui::End();
}

void RenderImGUI()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
#pragma endregion

int main()
{
	mainWindow = Window(1280,720);
	mainWindow.Init();

	CreateObjects();
	CreateShaders();

	//fov estandar = 45.0f
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);

	unsigned int uniformProjection = 0, uniformModel = 0;
	// Hacer un bucle hasta que la ventana se cierre

	InitIMGUI();

	while (!mainWindow.GetShouldClose())
	{
		//Conseguir y manejar los inputs de usuario
		glfwPollEvents();

		CreateIMGUIFrame();

		/*Poner color a la ventana y limpiarla --> http://docs.gl/gl3/glClearColor
		http://docs.gl/gl3/glClear */
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Limpia tanto el buffer de color como el de profundidad

		SetImGuiInterface();

		//Le decimos a OpenGL que vamos a usar estos shaders en específico --> http://docs.gl/gl3/glUseProgram
		shader->UseShader();
		
		MakeTransformations(model);
		
		shader->SetMatrixes(projection, model);

		mesh->RenderMesh();
		
		//Quitamos el programa
		glUseProgram(0);

		RenderImGUI();

		//Cogemos el buffer en el que hemos pintado el color rojo y lo ponemos en la pantalla
		//Al principio este buffer es invisible
		mainWindow.SwapBuffers();
	}

	DeletePointers();
	return 0;
}