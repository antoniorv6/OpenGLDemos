#pragma once
#include "GLFW/glfw3.h"
#include "Light.h"
class GUI
{
	public:
		GUI(GLFWwindow* c_windowToUse);
		~GUI();
		void SetGUI();
		void RenderGUI();
		void ResetGUI();
		void SetVariables(float&, float&, float&, float&, bool&, bool&, bool&, float&, float&, float&);
		void SetLightToHandle(Light* c_lightTohandle) {
			m_lightToHandle = c_lightTohandle;
		}

	private:
		void SetData();
		float xTranslate = 0.0f;
		float yTranslate = 0.0f;
		float zTranslate = -2.5f;
		float rotationAngle = 0.0f;

		bool xRotation = true;
		bool yRotation = false;
		bool zRotation = false;

		float xScale = .4f;
		float yScale = .4f;
		float zScale = .4f;
		
		//Light parameters
		float lightR = 1.0f;
		float lightG = 1.0f;
		float lightB = 1.0f;
		float lightInt = 0.7f;
		Light* m_lightToHandle;

		float lightX = 0.0f;
		float lightY = 0.0f;
		float lightZ = 0.0f;
		float diffInt = 0.7f;
};

