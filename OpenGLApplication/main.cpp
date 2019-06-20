#include <iostream>
#include "Window.h"
#include "Shader.h"
#include "Mesh.h"

const int WIDTH = 1280, HEIGHT = 720;

const char* vertexLocation = "Shaders/TestShader.vert";
const char* fragmentLocation = "Shaders/TestShader.frag";



void CreateMesh(Mesh* c_mesh)
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

	c_mesh->CreateMesh(vertices, indices, 12, 12);
}

int main()
{
	Window* l_window = new Window(WIDTH, HEIGHT);
	Shader* l_shader = new Shader();
	Mesh* l_mesh = new Mesh();

	l_shader->CreateShaders(vertexLocation, fragmentLocation);

	CreateMesh(l_mesh);

	while (!l_window->WindowHasToClose())
	{
		l_window->GetGLFWEvents();
		l_window->ClearWindow();

		l_shader->UseShader();
		l_mesh->RenderMesh();

		glUseProgram(0);

		l_window->SwapBuffers();
	}

	delete l_window;
	delete l_shader;
	delete l_mesh;
	l_window = nullptr;
	
	return 0;
}