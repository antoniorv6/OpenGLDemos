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

	//Le especificamos a OpenGL qué estructura tiene nuestro vector de datos -->http://docs.gl/es2/glVertexAttribPointer
	/*
		De momento el VAO está así:

			v1x | v1y | v1z | v2x | v2y | v2z ...

		Por lo que el atributo 0, que yo asigno a los vértices en el shader [con layout(location = 0) pos] tiene las siguientes propiedades:
		
		Nº Datos = 3
		Stride (Cuantos datos he de saltar para encontrar el siguiente vértice) = 0 (Porque son contínuos)
		Offset = 0 (Empieza al principio del vector)
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Le decimos a OpenGL que esto lo vamos a usar (muy XD) --> http://docs.gl/gl2/glEnableVertexAttribArray
	glEnableVertexAttribArray(0);

	//Liberamos todo lo que hemos estado ocupando en los buffers
	//Importante porque lo queremos usar para otras mallas y si no los liberamos... es una risa
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::RenderMesh()
{
	///Atención porque esto es importante a la par que gracioso
	//Le decimos a OpenGL que vamos a usar este VAO
	glBindVertexArray(VAO);
	//Le decimos a OpenGL que vamos a usar este IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	//Hace falta especificar el VBO (nuestros datos) ? NO, NO ES NECESARIO PORQUE EN OPENGL3 EL VBO ESTÁ CONTENIDO EN EL VAO
	//Este detalle es importantisimo y creo que Puchol no lo especifica

	//Dibujamos! --> http://docs.gl/gl3/glDrawElements
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	//Liberamos recursos por si queremos dibujar más cositas
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	//Proceso de borrado
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
