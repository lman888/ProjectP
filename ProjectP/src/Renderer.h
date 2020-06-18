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

	unsigned int VBO, VAO;

};