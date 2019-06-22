#pragma once
#include "GLFW/glfw3.h"
class GUI
{
	public:
		GUI(GLFWwindow* c_windowToUse);
		~GUI();
		void SetGUI();
		void RenderGUI();
		void ResetGUI();
		void SetVariables(float&, float&, float&, float&, bool&, bool&, bool&, float&, float&, float&);

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
};

