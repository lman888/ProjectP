#pragma once

#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* a_function, const char* a_file, int a_line);

class Renderer
{
public:

	void DrawTriangle();
	void GenerateTriangle();
	void Terminate();

protected:

private:
	/* Texture Data */
	unsigned int m_texture;
};