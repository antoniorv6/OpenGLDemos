#include "Mesh.h"
#include "Texture.h"

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
	std::cout << "Objeto de Mesh creado" << std::endl;
	m_texture = new Texture();
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

			v1x | v1y | v1z | u1 | v1 | v2x | v2y | v2z | u2 | v2...

		Por lo que el atributo 0, que yo asigno a los vértices en el shader [con layout(location = 0) pos] tiene las siguientes propiedades:
		
		Vértices:
		Nº Datos = 3
		Stride (Cuantos datos he de saltar para encontrar el siguiente vértice) = 5 (Ojo con las coordenadas de textura)
		Offset = 0 (Empieza al principio del vector)

		Coordenadas de textura:
		Nº Datos = 2
		Stride (Cuantos datos he de saltar para encontrar el siguiente vértice) = 5
		Offset = 3 * tamaño del dato (es 4, un float siempre es 4, pero lo hacemos fancy porque podemos)
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * 5, 0);

	//Habilitamos el vector de atributos de vértice que acabamos de crear --> http://docs.gl/gl2/glEnableVertexAttribArray
	glEnableVertexAttribArray(0);

	//Con esto manejamos las coordenadas de textura
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * 5, (void*)(4 * 3)); //El offset hay que meterle de 3, pq si no cuenta mal
	glEnableVertexAttribArray(1);
	
	//Liberamos todo lo que hemos estado ocupando en los buffers
	//Importante porque lo queremos usar para otras mallas y si no los liberamos... es una risa
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::SetTexture(const char* c_fileLocation)
{
	m_texture->LoadTexture(c_fileLocation);
}

void Mesh::RenderMesh()
{
	m_texture->UseTexture();
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
	delete m_texture;
	m_texture = nullptr;
	std::cout << "Objeto de Mesh destruido" << std::endl;
}
