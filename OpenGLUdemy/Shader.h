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

	void CreateFromFiles(const char*, const char*);

	std::string ReadFile(const char* fileLocation);

	void SetMatrixes(const glm::mat4& c_projection, const glm::mat4& c_model, const glm::mat4& c_view);

	void UseShader()	{ glUseProgram(m_shaderID);};
	void ClearShader() { if (m_shaderID != 0) { glDeleteProgram(m_shaderID); m_shaderID = 0; } m_uniformMVP = 0; }

	void AssignUniforms();

	unsigned int GetTextureUniform() { return m_TextureSampler; };

	~Shader();

private:
	unsigned int m_shaderID;
	unsigned int m_uniformMVP, m_TextureSampler;

	void CompileShader(const char*, const char*);
	void CreateShader(const char*, const char*);
	void LinkShader();
	void AddShader(unsigned int program, const char* shaderCode, GLenum shaderType);
};

