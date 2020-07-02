#pragma once

#include "GLAD/glad.h" /* OpenGL Headers */
#include "glm/glm.hpp" /* Math Headers */
#include "Renderer.h"


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

/* This Class holds the ID of the Shader Program. */
/* It also uses C++ filestreams to read the data from the Vert and Frag file into string objects */

class Shader
{
public:

	Shader(const char* a_vertexPath, const char* a_fragmentPath);
	~Shader();

	/* Use/Activate the Shader */
	void Bind();
	void UnBind();

	/* Set Uniform Functions */
	void SetBool(const std::string &a_name, bool a_value) const;
	void SetInt(const std::string &a_name, int a_value) const;
	void SetFloat(const std::string &a_name, float a_value) const;
	void SetUniformMat4F(const std::string& a_name, const glm::mat4& a_matrix);
	void SetUniform4f(const std::string& a_name, float v0, float v1, float v2, float v3);


	/* Deletes the Shader Program */
	void TerminateProgram();

protected:

private:

	/* The Shader Program ID */
	unsigned int ID;

	/* Gets the Uniforms Location */
	int GetUniformLocation(const std::string& a_name);

	std::unordered_map<std::string, int> m_UniformLocationCache;

};

