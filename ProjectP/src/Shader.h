#pragma once

#include "GLAD/glad.h" /* OpenGL Headers */

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/* This Class holds the ID of the Shader Program. */
/* It also uses C++ filestreams to read the data from the Vert and Frag file into string objects */

class Shader
{
public:

	Shader();

	void ConstructShaders(const char* a_vertexPath, const char* a_fragmentPath);

	/* Use/Activate the Shader */
	void UseProgram();

	/* Utility Form Functions */
	void SetBool(const std::string &a_name, bool a_value) const;
	void SetInt(const std::string &a_name, int a_value) const;
	void SetFloat(const std::string &a_name, float a_value) const;
	void TerminateProgram();

protected:

private:

	/* The Shader Program ID */
	unsigned int ID;

};

