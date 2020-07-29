#pragma once

#include "glm/glm.hpp"

/* Remember, the world moves around the Camera */

class Camera
{
public:
	Camera();

	/* Model to World Matrix */
	glm::mat4 GetWorldToViewMatrix() const;

protected:

private:

	/* Cameras initial Position in World Space */
	glm::vec3 m_cameraPosition = glm::vec3(0.0f, 0.0f, 0.3f);

	/* Where the camera is facing */
	glm::vec3 m_viewDirection;

	const glm::vec3 m_cameraUP;

};