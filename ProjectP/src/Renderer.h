#pragma once
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

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
		"layout(location = 0) in vec3 aPos; //The Position Variable has attribute position 0\n"
		"layout(location = 1) in vec3 aColor; //The Color Variable has attribute position 1\n;"
		"out vec3 ourColor; //Specify a color output to the fragment shader\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos, 1.0f); //See how we directly give a vec3 to a vec4's constructor\n"
		"   ourColor = aColor; //Sets the output variable to the input color we got from the Vertex Data\n"
		"}\n";

	const char* m_fragShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor; //Specify a color output to the fragment shader\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(ourColor, 1.0f);\n"
		"}\n";

	#pragma endregion

};