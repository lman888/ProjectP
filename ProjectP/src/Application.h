#pragma once
#include "GLAD/glad.h"
#include <gl/GL.h>
#include "GLFW/glfw3.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

class Application
{

public:
	

	int StartUp(void);

	int Update();

	void Terminate();

protected:


private:

	/* Calls the GLFW Window */
	GLFWwindow* m_window = NULL;

};