#pragma once

#include<GL\glew.h>
#include<iostream>
class Texture;

class Mesh
{
public:
	Mesh();

	void CreateMesh(float *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void SetTexture(const char* c_fileLocation);
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	unsigned int VAO, VBO, IBO;
	GLsizei indexCount;
	Texture* m_texture;
};

