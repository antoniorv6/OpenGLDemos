#define STB_IMAGE_IMPLEMENTATION 
#include "Texture.h"
#include "stb_image.h"
#include "GL/glew.h"
#include <iostream>

Texture::Texture()
{
	m_textureW = 0;
	m_textureH = 0;
	m_textureBD = 0;
}

void Texture::LoadTexture(const char* c_location)
{
	m_texture = stbi_load(c_location, &m_textureW, &m_textureH, &m_textureBD, 4);
	if (!m_texture)
	{
		std::cout << "[ERROR] - Texture could not be found"<<std::endl;
		stbi_image_free(m_texture);
		return;
	}

	//La textura ya está cargada en memoria RAM
	BindTexture();
}

void Texture::BindTexture()
{
	//Reservar memoria en la gráfica para la textura
	glGenTextures(1, &m_id);
	
	//Asignarle a la textura un typedef
	glBindTexture(GL_TEXTURE_2D, m_id);

	//Vamos a poner varios parámetros de configuración de la textura --> http://docs.gl/gl3/glTexParameter

	/*
		Primero decimos que si las coordenadas de textura están fuera
		de los boundaries de la textura original, esta se irá clonando para salvar estos espacios
		Hay que especificarlo tanto para el eje x (s) como para el y (t)
		Son parámetros necesarios para que luego OpenGL pueda generar los MipMaps de textura
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/*
		Ahora especificamos cómo queremos que OpenGL trate la textura en caso de que nos acerquemos (MIN_FILTER) o alejemos (MAG_FILTER)
		a ella.
		Podemos hacer 2 cosas -> Desenfocar (Lineal)
								 Pixelar (Nearest)
		Son parámetros necesarios para que luego OpenGL pueda generar los MipMaps de textura
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/*
		Asignamos a este id de textura en la gráfica los datos que hemos cargado de la imagen. --> http://docs.gl/gl3/glTexImage2D
		Aclaración -> ¿Por qué GL_UNSIGNED_BYTE? Porque estamos cargando la imagen con unsigned char*, que es lo más común. Un char, es un byte en la memoria
		Todo esto depende de nuestra librería de cargado de imágenes, cuando usamos unsigned char* en realidad estamos cargando la imagen
		byte a byte, por lo que hemos de usar UNSIGNED_BYTE para que OpenGL reconozca bien este tipo de datos.
		En caso de hacerlo de otra forma o con otra libreria que trabaje con otro tipo de datos, hay que cambiarlo.
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_textureW, m_textureH, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_texture);

	//Generamos el mipmap para el mapeo de la textura con la distancia
	glGenerateMipmap(GL_TEXTURE_2D);

	//Liberamos memoria de la RAM y unbindeamos por si queremos crear otra textura
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(m_texture);
}

void Texture::UseTexture()
{
	//Le decimos a OpenGL que vamos a usar la textura 0
	glActiveTexture(GL_TEXTURE0); //Esto a efectos prácticos es equivalente a glGetUniformLocation() y glSetUniform1f()
	
	//Le decimos que la textura 0 es la que tiene reservada en memoria con id el que me haya dado
	glBindTexture(GL_TEXTURE_2D, m_id);
}
