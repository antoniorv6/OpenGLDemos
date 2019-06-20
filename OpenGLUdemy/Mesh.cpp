#include "Mesh.h"

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
	std::cout << "Objeto de Mesh creado" << std::endl;
}

void Mesh::CreateMesh(float *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;

	//Obtenemos el ID para nuestro VAO --> http://docs.gl/gl3/glGenVertexArrays
	glGenVertexArrays(1, &VAO);

	//Asignamos ese id --> http://docs.gl/gl3/glBindVertexArray
	glBindVertexArray(VAO);

	/*A partir de aquí, todas las operaciones que hagamos con un VAO van a ser con el que tiene ese id que
	hemos generado previamente*/

	//Generamos el ID del VBO --> http://docs.gl/gl3/glGenBuffers
	glGenBuffers(1, &VBO);

	//Generamos el ID del IBO
	glGenBuffers(1, &IBO);
	//Decimos que tipo de buffer es
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//Metemos los datos que queremos
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);


	//Asignamos ese id --> http://docs.gl/gl3/glBindBuffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Linkar los vértices al buffer que tenemos asignado --> http://docs.gl/gl3/glBufferData
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	//Especificamos la localización y el formato de los datos que contiene el array de atributos de vértices -->http://docs.gl/es2/glVertexAttribPointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Habilitamos el vector de atributos de vértice que acabamos de crear --> http://docs.gl/gl2/glEnableVertexAttribArray
	glEnableVertexAttribArray(0);

	//Liberamos todo lo que hemos estado ocupando en los buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::RenderMesh()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	//Dibujamos --> http://docs.gl/gl3/glDrawElements
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

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
	std::cout << "Objeto de Mesh destruido" << std::endl;
}
