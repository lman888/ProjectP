#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

/* Remember, the world moves around the Camera */

class Camera
{
public:
	/* Initialises the Cameras Variables */
	Camera() : m_cameraPosition(-10.0f, 0.0f, 0.3f), m_cameraFront(0.0f, 0.0f, -1.0f),
			   m_cameraUp(0.0f, 1.0f, 0.0f), m_cameraSpeed(10.0f),
		       m_cameraSpeedValue(0.0f), m_pitch(0.0f), m_yaw(0.0f), 
			   m_view(0.0f), m_fov(45.0f){};

	~Camera() {};

	/* Camera Rotation in our Application */
	void CameraRotation(float a_xOffSet, float a_yOffSet)
	{
		/* Pitch Constraints */
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;

		/* Add the Offset Values */
		m_yaw += a_xOffSet;
		m_pitch += a_yOffSet;

		/* Calculates the Direction Vector */
		glm::vec3 m_direction;
		m_direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_direction.y = sin(glm::radians(m_pitch));
		m_direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_cameraFront = glm::normalize(m_direction);
	}

	/* Camera Movement in our Application */
	void CameraInputs(GLFWwindow* a_window, float a_cameraSpeedValue, float a_deltaTime)
	{
		m_view = glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp);

		m_cameraSpeed = a_cameraSpeedValue * a_deltaTime;
		if (glfwGetKey(a_window, GLFW_KEY_W) == GLFW_PRESS)
			m_cameraPosition += m_cameraSpeed * m_cameraFront;
		if (glfwGetKey(a_window, GLFW_KEY_S) == GLFW_PRESS)
			m_cameraPosition -= m_cameraSpeed * m_cameraFront;
		if (glfwGetKey(a_window, GLFW_KEY_A) == GLFW_PRESS)
			m_cameraPosition -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * m_cameraSpeed;
		if (glfwGetKey(a_window, GLFW_KEY_D) == GLFW_PRESS)
			m_cameraPosition += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * m_cameraSpeed;
	}
	
	float GetCameraFOV()
	{
		return m_fov;
	}
	
	/* Returns the Camera's View Matrix */
	glm::mat4 GetViewMatrix()
	{
		return m_view;
	}

protected:

private:

	/* Camera Initial Variables */
	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraUp;
	float m_pitch;
	float m_yaw;
	float m_fov;

	/* Cameras View */
	glm::mat4 m_view;

	/* Camera's Speed */
	float m_cameraSpeed;
	float m_cameraSpeedValue;

};