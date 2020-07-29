#include "Camera.h"
#include "glm/gtx/transform.hpp"

Camera::Camera() : m_viewDirection(0.0f, 0.0f, 0.0f), m_cameraUP(0.0f, 1.0f, 0.0f)
{
}

glm::mat4 Camera::GetWorldToViewMatrix() const
{
	return glm::lookAt(m_cameraPosition, m_cameraPosition + m_viewDirection, m_cameraUP);
}
