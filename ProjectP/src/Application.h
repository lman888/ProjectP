#pragma once

#include "Extra/imgui/imgui.h"
#include "Extra/imgui/imgui_impl_glfw.h"
#include "Extra/imgui/imgui_impl_opengl3.h"

#include "GLAD/glad.h"
#include <gl/GL.h>
#include "GLFW/glfw3.h"

#include "Camera.h"
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

};