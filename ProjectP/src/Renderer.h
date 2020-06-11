#pragma once
#include "GLAD/glad.h"

#include <iostream>

class Renderer
{
public:

	void DrawTriangle();

	void CompileShader();

protected:

private:

	#pragma region Shader Code
	
	/* Vertex Shader ID */
	unsigned int m_vertexShader;
	/* Fragment Shader ID */
	unsigned int m_fragShader;
	/* Shader Program ID */
	unsigned int m_shaderProgram;

	/* Checks if the Shader compiled successfully */
	int m_success = 0;
	char m_infoLog[512];


	const char* m_vertexShaderSource = "#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
		"}\n";

	const char* m_fragShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n";

	#pragma endregion

};