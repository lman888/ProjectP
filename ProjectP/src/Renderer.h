#pragma once

#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

class Renderer
{
public:

	void DrawTriangle();
	void GenerateTriangle();
	void Terminate();

protected:

private:

	/* Vertex, Element and Array Buffer ID's */
	unsigned int VBO, VAO, EBO;
	/* Texture Data */
	unsigned int m_texture;
};