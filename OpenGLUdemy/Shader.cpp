#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformMVP = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char * vertexCode = vertexString.c_str();
	const char * fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
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

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	//Obtenemos el id del programa vacio --> http://docs.gl/gl3/glCreateProgram
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		std::cout << "Error creando el programa de shader" << std::endl;
		return;
	}

	//Añadimos nuestros Shaders al programa
	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	int result = 0;
	char elog[1024] = { 0 };

	//Creamos los ejecutables en la GPU --> http://docs.gl/gl3/glLinkProgram
	glLinkProgram(shaderID);
	//Obtenemos el resultado de el linkado --> http://docs.gl/gl3/glGetProgram
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

	if (!result)
	{
		//Si falla, pillamos el log que ha dejado OpenGL con el error --> http://docs.gl/gl3/glGetProgramInfoLog
		glGetProgramInfoLog(shaderID, sizeof(elog), NULL, elog);
		std::cout << "Error linkando el programa: " << *elog << std::endl;
		return;
	}

	//Validamos el shader, por si hemos hecho algo mal --> http://docs.gl/gl3/glValidateProgram
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		//Si falla, pillamos el log que ha dejado OpenGL con el error --> http://docs.gl/gl3/glGetProgramInfoLog
		glGetProgramInfoLog(shaderID, sizeof(elog), NULL, elog);
		std::cout << "Error validando el programa: " << elog << std::endl;
		return;
	}

	//Conseguir el id del uniform en el shader --> http://docs.gl/gl3/glGetUniformLocation
	uniformMVP = glGetUniformLocation(shaderID, "MVP");
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

	//Obtenemos el resultado de el linkado --> http://docs.gl/gl3/glGetProgram
	glGetShaderiv(c_shader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		//Si falla, pillamos el log que ha dejado OpenGL con el error --> http://docs.gl/gl3/glGetShaderInfoLog
		glGetShaderInfoLog(c_shader, sizeof(elog), NULL, elog);
		std::cout << "Error compilando el " << shaderType << ": " << elog << std::endl;
		return;
	}

	//Juntamos el shader al programa --> http://docs.gl/gl3/glAttachShader
	glAttachShader(program, c_shader);
}

void Shader::SetMatrixes(glm::mat4& c_projection, glm::mat4& c_model)
{
	glm::mat4 result = c_projection * c_model;
	glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(result));
}

Shader::~Shader()
{
	ClearShader();
}
