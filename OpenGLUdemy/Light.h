#pragma once
#include "LightData.h"
#include <glm/glm.hpp>
class Light
{
	public:
		Light();
		Light(LightData c_uniforms);

		void RenderLight();

		void SetNewLightData(LightParameters c_newParameters);
	
	private:
		
		LightData m_uniformsToUse;

		//Luz ambiente
		glm::vec3 m_lightColor;
		float	  m_ambientIntensity;
		//----------------------------

		//Luz difusa
		glm::vec3 m_lightPosition;
		float	  m_diffuseIntensity;
		//----------------------------

};

