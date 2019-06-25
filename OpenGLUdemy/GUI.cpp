#include "GUI.h"
#include "IMGUIuse/imgui.h"
#include "IMGUIuse/imgui_impl_opengl3.h"
#include "IMGUIuse/imgui_impl_glfw.h"

GUI::GUI(GLFWwindow* c_windowToUse)
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(c_windowToUse, true);
	ImGui_ImplOpenGL3_Init("#version 150");
}

GUI::~GUI()
{
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
}

void GUI::SetGUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	SetData();
}

void GUI::SetData()
{
	ImGui::SetNextWindowSize(ImVec2(300, 400));
	ImGui::Begin("Controles increibles");
	ImGui::Text("Translacion");
	ImGui::SliderFloat(" X", &xTranslate, -5.0f, 5.0f);
	ImGui::SliderFloat(" Y", &yTranslate, -5.0f, 5.0f);
	ImGui::SliderFloat(" Z", &zTranslate, -5.0f, 5.0f);

	ImGui::Separator();
	ImGui::Text("Rotacion");
	ImGui::SliderAngle(" Angulo", &rotationAngle, -360.0f, 360.0f);
	ImGui::Checkbox("Eje X", &xRotation);
	ImGui::Checkbox("Eje Y", &yRotation);
	ImGui::Checkbox("Eje Z", &zRotation);

	ImGui::Separator();
	ImGui::Text("Escalado");
	ImGui::SliderFloat(" XS", &xScale, -5.0f, 5.0f);
	ImGui::SliderFloat(" YS", &yScale, -5.0f, 5.0f);
	ImGui::SliderFloat(" ZS", &zScale, -5.0f, 5.0f);

	if (ImGui::Button("Reset", ImVec2(70, 30)))
	{
		ResetGUI();
	}


	ImGui::Separator();
	ImGui::Text("Parametros de la luz");
	ImGui::SliderFloat(" R", &lightR, 0.0f, 1.0f);
	ImGui::SliderFloat(" G", &lightG, 0.0f, 1.0f);
	ImGui::SliderFloat(" B", &lightB, 0.0f, 1.0f);
	ImGui::SliderFloat(" Ambience", &lightInt, 0.0f, 1.0f);

	ImGui::SliderFloat(" LX", &lightX, -10.0f, 10.0f);
	ImGui::SliderFloat(" LY", &lightY, -10.0f, 10.0f);
	ImGui::SliderFloat(" LZ", &lightZ, -10.0f, 10.0f);

	ImGui::SliderFloat(" Diffuse", &diffInt, 0.0f, 1.0f);

	ImGui::Separator();
	ImGui::Text("Otras cosas");

	if (ImGui::Button("Habilitar culling", ImVec2(150, 30)))
	{
		glEnable(GL_CULL_FACE);
	}

	if (ImGui::Button("Deshabilitar culling", ImVec2(150, 30)))
	{
		glDisable(GL_CULL_FACE);
	}


	ImGui::End();
}

void GUI::ResetGUI()
{
	xTranslate = 0.0f;
	yTranslate = 0.0f;
	zTranslate = -2.5f;
	rotationAngle = 0.0f;

	xRotation = true;
	yRotation = false;
	zRotation = false;

	xScale = .4f;
	yScale = .4f;
	zScale = .4f;
}

void GUI::RenderGUI()
{
	LightParameters l_newData = LightParameters();
	l_newData.s_color = glm::vec3(lightR, lightG, lightB);
	l_newData.s_intensity = lightInt;
	l_newData.s_lightPos = glm::vec3(lightX, lightY, lightZ);
	l_newData.s_diffIntensity = diffInt;

	m_lightToHandle->SetNewLightData(l_newData);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::SetVariables(float& c_Tx, float& c_Ty, float& c_Tz, float& c_Angle, bool& c_RotX, bool& c_RotY, bool& c_RotZ, float& c_Sx, float& c_Sy, float& c_Sz)
{
	c_Tx = xTranslate;
	c_Ty = yTranslate;
	c_Tz = zTranslate;

	c_Angle = rotationAngle;
	c_RotX = xRotation;
	c_RotY = yRotation;
	c_RotZ = zRotation;

	c_Sx = xScale;
	c_Sy = yScale;
	c_Sz = zScale;
}

