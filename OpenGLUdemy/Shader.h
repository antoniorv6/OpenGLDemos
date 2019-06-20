#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>

#include <GL\glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>


class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);

	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	void SetMatrixes(glm::mat4& c_projection, glm::mat4& c_model);

	void UseShader()	{ glUseProgram(shaderID);};
	void ClearShader() { if (shaderID != 0) { glDeleteProgram(shaderID); shaderID = 0; } uniformMVP = 0; }

	~Shader();

private:
	unsigned int shaderID, uniformMVP;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(unsigned int program, const char* shaderCode, GLenum shaderType);
};

