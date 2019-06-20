#pragma once

#include<GL\glew.h>
#include<iostream>

class Mesh
{
public:
	Mesh();

	void CreateMesh(float *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	unsigned int VAO, VBO, IBO;
	GLsizei indexCount;
};

