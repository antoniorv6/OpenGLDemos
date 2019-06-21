#include "Shader.h"

Shader::Shader()
{
	m_shaderID = 0;
	m_uniformMVP = 0;
}

void Shader::CreateFromFiles(const char* c_vertexfileLocation, const char* c_fragmentfileLocation)
{
	//Guardamos dos strings con el código de nuestros shaders
	//Why? Porque OpenGL parsea de string, mejor dicho, de punteros a caracteres, ya que tiene su propio compilador y linkador
	std::string vertexString = ReadFile(c_vertexfileLocation);
	std::string fragmentString = ReadFile(c_fragmentfileLocation);
	
	//Transformamos a const char*
	const char * l_vertexCode = vertexString.c_str();
	const char * l_fragmentCode = fragmentString.c_str();

	//Entramos a la rutina de creación del shader
	CreateShader(l_vertexCode, l_fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		std::cout << "Failed to read " << fileLocation << " file does not exist" << std::endl;
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

void Shader::CreateShader(const char* c_vertexCode, const char* c_fragmentCode)
{
	//Esta es nuestra nuestra función CORE que compilará y linkará el shader
	//Compilamos el Shader
	CompileShader(c_vertexCode, c_fragmentCode);
	//Una vez compilado, hay que mapearlo en memoria --> LINKAR
	//También validamos en esta función
	LinkShader();
	//Ahora que todo está bien, asignamos los uniforms que vamos a usar en los shaders
	AssignUniforms();
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	//Obtenemos el id del programa vacio --> http://docs.gl/gl3/glCreateProgram
	m_shaderID = glCreateProgram();

	if (!m_shaderID)
	{
		std::cout << "Error creando el programa de shader" << std::endl;
		return;
	}

	//Añadimos nuestros Shaders al programa
	AddShader(m_shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(m_shaderID, fragmentCode, GL_FRAGMENT_SHADER);
}

void Shader::LinkShader()
{
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
}

void Shader::AddShader(unsigned int program, const char* shaderCode, GLenum shaderType)
{
	//Generamos el shader vacio y obtenemos el ID --> http://docs.gl/gl3/glCreateShader
	unsigned int c_shader = glCreateShader(shaderType);

	const char* c_code[1];
	c_code[0] = shaderCode;

	int codeLength[1];
	codeLength[0] = strlen(shaderCode);

	//Asignamos el código al Shader --> http://docs.gl/gl3/glShaderSource
	glShaderSource(c_shader, 1, c_code, codeLength);

	int result = 0;
	char elog[1024] = { 0 };
	//Compilamos el shader
	glCompileShader(c_shader);

	//Obtenemos el resultado de la compilación --> http://docs.gl/gl3/glGetProgram
	glGetShaderiv(c_shader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		//Si falla, pillamos el log que ha dejado OpenGL con el error --> http://docs.gl/gl3/glGetShaderInfoLog
		glGetShaderInfoLog(c_shader, sizeof(elog), NULL, elog);
		std::cout << "Error compilando el " << shaderType << ": " << elog << std::endl;
		return;
	}

	//Juntamos el shader al programa --> http://docs.gl/gl3/glAttachShader
	//Es decir, le decimos a OpenGL que el shader va a estar dentro del programa general
	glAttachShader(program, c_shader);
}

void Shader::AssignUniforms()
{
	//Conseguir el id del uniform en el shader --> http://docs.gl/gl3/glGetUniformLocation
	m_uniformMVP = glGetUniformLocation(m_shaderID, "MVP");
}

void Shader::SetMatrixes(glm::mat4& c_projection, glm::mat4& c_model)
{
	///Esto es una optimización a nivel de CPU
	//Multiplicamos la Proyección por la Model
	//ESTA MULTIPLICACIÓN SIEMPRE ES ASÍ
	glm::mat4 result = c_projection * c_model;
	
	//Le decimos al shader que le vamos a pasar una matriz de 4*4 con floats a un uniform que tiene el ID que indicamos
	//con m_uniformMVP
	glUniformMatrix4fv(m_uniformMVP, 1, GL_FALSE, glm::value_ptr(result));
}

Shader::~Shader()
{
	ClearShader();
}
