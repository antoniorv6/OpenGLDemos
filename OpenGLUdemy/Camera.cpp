#include "Camera.h"
#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>

Camera::Camera()
{
	//glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.3f
	//Inicialización de los parámetros de la cámara
	m_position = glm::vec3(0.0f, 0.0f, 0.0f); //Posición de la cámara
	m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);  //Vector UP del mundo (importante para saber cómo rota la cámara)
	m_front = glm::vec3(0.0f, 0.0f, -1.0f);

	UpdateCamera();
}

void Camera::HandleKeys(bool * c_keys)
{
	bool* l_keys = c_keys;
	//Handle de las teclas, no es lo mejor, pero nos funciona
	if (l_keys[GLFW_KEY_W])
	{
		m_position += m_front * m_movementSpeed;
	}
	if (l_keys[GLFW_KEY_D])
	{
		m_position += m_right * m_movementSpeed;
	}
	if (l_keys[GLFW_KEY_A])
	{
		m_position -= m_right * m_movementSpeed;
	}
	if (l_keys[GLFW_KEY_S])
	{
		m_position -= m_front * m_movementSpeed;
	}
}

void Camera::UpdateCamera()
{
	m_front = glm::normalize(m_front); //Lo normalizamos porque queremos cambiar él sentido, no modificar las direcciones

	m_right = glm::normalize(glm::cross(m_front, m_worldUp)); //Producto vectorial entre la dirección de la cámara (front) y el vector de arriba del mundo
	m_up = glm::normalize(glm::cross(m_right, m_front));      //Producto vectorial entre el vector derecha y el frontal

	m_viewMatrix = glm::lookAt(m_position, m_front + m_position, m_up); //glm::lookat genera la matriz de vista para nuestra escena
}
