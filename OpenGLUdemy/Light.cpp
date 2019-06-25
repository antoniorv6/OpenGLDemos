#include "Light.h"
#include <GL/glew.h>

Light::Light()
{
}

Light::Light(LightData c_uniforms)
{
	m_uniformsToUse = c_uniforms;
	m_lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	m_ambientIntensity = 0.7f;
	m_lightPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	m_diffuseIntensity = 0.7f;
}

void Light::RenderLight()
{
	glUniform3f(m_uniformsToUse.s_UniformAmbientColor, m_lightColor.x, m_lightColor.y, m_lightColor.z);
	glUniform3f(m_uniformsToUse.s_UniformLightPosition, m_lightPosition.x, m_lightPosition.y, m_lightPosition.z);

	glUniform1f(m_uniformsToUse.s_UniformAmbientIntensity, m_ambientIntensity);
	glUniform1f(m_uniformsToUse.s_UniformLightDiffuseIntesity, m_diffuseIntensity);
}

void Light::SetNewLightData(LightParameters c_newParameters)
{
	m_lightColor = c_newParameters.s_color;
	m_ambientIntensity = c_newParameters.s_intensity;
	m_lightPosition = c_newParameters.s_lightPos;
	m_diffuseIntensity = c_newParameters.s_diffIntensity;
}
