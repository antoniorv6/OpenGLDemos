#include "Mesh.h"

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
	std::cout << "Objeto de Mesh creado" << std::endl;
}

void Mesh::CreateMesh(float* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;

	//Generamos el VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Generamos el VBO
	glGenBuffers(1, &VBO);

	//Generamos el IBO
	glGenBuffers(1, &IBO);

	//Decimos de que tipo, es como hacer un typedef
	//Introducmios los indices que vamos a usar en nuestro buffer de indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	//Especificamos como vamos a llamar al VBO
	//Introducmios los datos que vamos a usar en nuestro buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	//Especificamos las "Instrucciones" para que trate bien los datos
	//Y lo habilitamos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//Liberamos para otras mallas
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::cout << "Mesh creada" << std::endl;

}

void Mesh::RenderMesh()
{
	//Pillamos los tipos
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	//Dibujamos --> http://docs.gl/gl3/glDrawElements
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	//Liberamos 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO); //Borramos el buffer de la tarjeta gráfica
		IBO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO); //Borramos el buffer de la tarjeta gráfica
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO); //Borramos el buffer de la tarjeta gráfica
		VAO = 0;
	}

	indexCount = 0;

	std::cout << "Objeto de Mesh limpiado" << std::endl;
}

Mesh::~Mesh()
{
	ClearMesh();
	std::cout << "Mesh destruido" << std::endl;
}
