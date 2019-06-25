#pragma once
#include <glm/glm.hpp>
class Window;
class Camera
{
	public:
		Camera();
		glm::mat4 GetViewMatrix() { return m_viewMatrix; };
		void HandleKeys(bool* c_keys);
		void UpdateCamera();

		glm::vec3 GetCameraPosition() { return m_position; };

		
	private:
		Window* m_windowToHandle;
		glm::vec3 m_position, m_front, m_up, m_worldUp, m_right; //Vectores esenciales de la cámara
		float m_movementSpeed = .1f;
		glm::mat4 m_viewMatrix;
};

