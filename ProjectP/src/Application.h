#pragma once

#include "Extra/imgui/imgui.h"
#include "Extra/imgui/imgui_impl_glfw.h"
#include "Extra/imgui/imgui_impl_opengl3.h"

#include "GLAD/glad.h"
#include <gl/GL.h>
#include "GLFW/glfw3.h"

#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

class Application
{

public:
	
	/* Initialises the Window */
	int StartUp(void);

	/* Updates the Window and Objects in it */
	int Update();

	/* Stops the OpenGL Application */
	void Terminate();

	void CameraInputs(GLFWwindow *a_window);

protected:


private:

	/* Calls the GLFW Window */
	GLFWwindow* m_window = NULL;

	/* Camera Starting Position */
	glm::vec3 m_cameraPosition = glm::vec3(0.0f, 0.0f, 0.3f);
	glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 m_view;

	float m_cameraSpeed;
	float m_cameraSpeedValue;
};