#pragma once
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <iostream>

class Application
{

public:

	int StartUp(void);

	int Update();

	int Terminate();

protected:


private:

	/* Calls the GLFW Window */
	GLFWwindow* m_window = NULL;

	/* Calls the Renderer */
	Renderer m_renderer;

};