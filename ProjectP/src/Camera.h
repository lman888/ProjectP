#pragma once

#include "glm/glm.hpp"

class Camera
{
public:

protected:

private:

	/* Cameras initial Position in World Space */
	glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 0.3f);

	/* Where the camera is facing (Centre of the Scene) */
	glm::vec3 m_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	/* Subtracting the Target and Pos Vector will give us the Direction Vec */
	glm::vec3 m_cameraDir = glm::normalize(m_cameraPos - m_cameraTarget);

	/* Camera Vectors */
	/* This vector specifies the Upwards vector that points upwards in World Space */
	glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	/* This new Vector is the cross productive of the Up and Dir Vector which will give us */
	/* a Vector that points in the Positive X-Axis direction */
	glm::vec3 m_cameraRight = glm::normalize(glm::cross(m_up, m_cameraDir));
	/* Since we have both X and Z Axis Vectors - We take the cross product of the Right and Direction Vector */
	glm::vec3 m_cameraUp = glm::cross(m_cameraDir, m_cameraRight);


};