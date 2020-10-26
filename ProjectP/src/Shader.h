#pragma once

#include "GLAD/glad.h" /* OpenGL Headers */
#include "glm/glm.hpp" /* Math Headers */

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

/* This Class holds the ID of the Shader Program. */
/* It also uses C++ filestreams to read the data from the Vert and Frag file into string objects */

/* Custom Exception Class for Errors */
class GLSLProgramException : public std::runtime_error
{
public:
	GLSLProgramException(const std::string& msg)
		: std::runtime_error(msg) {}
};

/* Various Shader Types */
namespace GLSLShader
{
	enum GLSLShaderType
	{
		/* Shader Types */
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
		GEOMETRY = GL_GEOMETRY_SHADER,
		TESS_CONTROL = GL_TESS_CONTROL_SHADER,
		TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
		COMPUTER = GL_COMPUTE_SHADER_BIT
	};
}

class Shader
{
public:

	Shader() : m_linked(false), m_handle(0) {};
	~Shader();

	/* Makes The Program Non Copyable */
	Shader(const Shader &) = delete;
	Shader & operator=(const Shader&) = delete;

	/* Shader Compile Functions */
	void CompileShader(const char * a_fileName);
	void CompileShader(const char * a_fileName,
					   GLSLShader::GLSLShaderType a_type);
	void CompileShader(const std::string & a_source, GLSLShader::GLSLShaderType a_type,
		               const char * a_fileName = nullptr);

	/* Use/Activate the Shader */
	void Bind() const;
	void Link();
	void Validate();
	void UnBind() const;

	/* Return Functions */
	int GetHandle() { return m_handle; }
	bool IsLinked() { return m_linked; }


	void BindAttribLocation(unsigned int a_location, const char* a_name);
	void BindFragDataLocation(unsigned int a_location, const char* a_name);
	void PrintActiveAttribs();
	void PrintActiveUniforms();

	/* Set Uniform Functions */
	void SetUniform1f(const std::string &a_name, float a_value);
	void SetUniform1i(const std::string& a_name, int a_value);
	void SetUniformMat4f(const std::string& a_name, const glm::mat4& a_matrix);
	void SetUniform4f(const std::string& a_name, float v0, float v1, float v2, float v3);
	void SetUniformMat3f(const std::string& a_name, glm::mat3& a_value);
	void SetUniform1b(const std::string& a_name, bool a_value);
	void SetUniformVec3f(const std::string& a_name, const glm::vec3& a_value);
	void SetUniformVec4f(const std::string& a_name, const glm::vec4& a_value);

	void PracticePipleLine();
	void UniformBlock();

	/* Deletes the Shader Program */
	void TerminateProgram();
	void DetachAndDeleteShaders();




	void CreateShaderPrograms();

protected:

private:

	/* The Shader Program ID */
	unsigned int m_handle;
	/* Checks if Program has been linked */
	bool m_linked;
	/* Gets the Uniforms Name */
	int GetUniformLocation(const std::string& a_name);
	/* Holds the Uniforms Cache Location */
	std::unordered_map<std::string, int> m_UniformLocationCache;
	std::string LoadShaderAsString(const char* a_fileName);

	const char *GetValueTypeString(GLenum a_type);
};

