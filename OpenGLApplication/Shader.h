#pragma once
#include <string>
#include <GL/glew.h>

class Shader
{
	public:
		Shader();
		~Shader();
		std::string ReadFile(const char* c_fileLocation);
		void CreateShaders(const char* c_vertexCode, const char* c_fragmentCode);
		void CompileShader(const char* vertexCode, const char* fragmentCode);
		void IncludeShader(unsigned int c_program, const char* c_programCode, GLenum c_shaderType);
		void UseShader() { glUseProgram(m_shaderID); };

    private:
		unsigned int m_shaderID;
};

