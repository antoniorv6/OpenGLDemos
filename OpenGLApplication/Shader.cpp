#include "Shader.h"
#include <fstream>
#include <iostream>

Shader::Shader()
{
	m_shaderID = 0;
}

Shader::~Shader()
{
	if (m_shaderID != -1) 
	{ 
		glDeleteProgram(m_shaderID);
	}
}

std::string Shader::ReadFile(const char* c_fileLocation)
{
	std::string content;
	std::ifstream fileStream(c_fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		std::cout << "Failed to read " << c_fileLocation << " file does not exist" << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::CreateShaders(const char* c_vertexCode, const char* c_fragmentCode)
{
	std::string vertexString = ReadFile(c_vertexCode);
	std::string fragmentString = ReadFile(c_fragmentCode);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	//Crear nuestro programa que va a ejecutar el shader!!
	std::cout << "Creating shaders..." << std::endl;
	m_shaderID = glCreateProgram();

	if (!m_shaderID)
	{
		std::cout << "Error creando el programa de shader" << std::endl;
		return;
	}

	//Compilamos y añadimos los shaders al programa
	IncludeShader(m_shaderID, vertexCode, GL_VERTEX_SHADER);
	IncludeShader(m_shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	//Una vez hecho eso, toca linkar el programa
	int result = 0;
	char elog[1024] = { 0 };

	//Creamos los ejecutables en la GPU --> http://docs.gl/gl3/glLinkProgram
	glLinkProgram(m_shaderID);
	//Obtenemos el resultado de el linkado --> http://docs.gl/gl3/glGetProgram
	glGetProgramiv(m_shaderID, GL_LINK_STATUS, &result);

	if (!result)
	{
		//Si falla, pillamos el log que ha dejado OpenGL con el error --> http://docs.gl/gl3/glGetProgramInfoLog
		glGetProgramInfoLog(m_shaderID, sizeof(elog), NULL, elog);
		std::cout << "Error linkando el programa: " << *elog << std::endl;
		return;
	}

	//Validamos el shader, por si hemos hecho algo mal --> http://docs.gl/gl3/glValidateProgram
	glValidateProgram(m_shaderID);
	glGetProgramiv(m_shaderID, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		//Si falla, pillamos el log que ha dejado OpenGL con el error --> http://docs.gl/gl3/glGetProgramInfoLog
		glGetProgramInfoLog(m_shaderID, sizeof(elog), NULL, elog);
		std::cout << "Error validando el programa: " << elog << std::endl;
		return;
	}

	std::cout << "Created!" << std::endl;
}

void Shader::IncludeShader(unsigned int c_program, const char* c_programCode, GLenum c_shaderType)
{
	//Generamos el shader con su identificador
	unsigned int l_shaderID = glCreateShader(c_shaderType);

	//Mierdas para parsear el codigo a OpenGL
	const char* l_code[1];
	l_code[0] = c_programCode;

	int l_codelength[1];
	l_codelength[0] = strlen(c_programCode);

	//Especificar el código que va a tener el shader
	glShaderSource(l_shaderID, 1, l_code, l_codelength);

	//Depuración de errores
	int result = 0;
	char elog[1024] = { 0 };

	//Compilamos el shader
	glCompileShader(l_shaderID);

	//Obtenemos el resultado de la compilación --> http://docs.gl/gl3/glGetProgram
	glGetShaderiv(l_shaderID, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		//Si falla, pillamos el log que ha dejado OpenGL con el error --> http://docs.gl/gl3/glGetShaderInfoLog
		glGetShaderInfoLog(l_shaderID, sizeof(elog), NULL, elog);
		std::cout << "Error compilando el " << c_shaderType << ": " << elog << std::endl;
		return;
	}

	//Si llegamos aquí, el shader ha compilado correctamente
	glAttachShader(c_program, l_shaderID);
}


