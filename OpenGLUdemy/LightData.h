#pragma once
#include <glm/glm.hpp>

struct LightData
{
	//Datos luz ambiente
	unsigned int s_UniformAmbientColor;
	unsigned int s_UniformAmbientIntensity;
	unsigned int s_UniformLightPosition;
	unsigned int s_UniformLightDiffuseIntesity;
};

struct LightParameters
{
	float s_intensity;
	glm::vec3 s_color;

	float s_diffIntensity;
	glm::vec3 s_lightPos;
};
