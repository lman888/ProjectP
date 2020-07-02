#include "Shader.h"

Shader::Shader(const char* a_vertexPath, const char* a_fragmentPath)
{
	/* Retrieves the Vertex/Fragment source code from the File Path */
	/* Vert Code Path */
	std::string vertexCode;
	/* Frag Code Path */
	std::string fragmentCode;
	/* Vert Shader File */
	std::ifstream vShaderFile;
	/* Frag Shader File */
	std::ifstream fShaderFile;

	/* Ensure ifstream objects can throw exeptions */
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		/* Opens Files */
		vShaderFile.open(a_vertexPath);
		fShaderFile.open(a_fragmentPath);
		/* File Stream holders */
		std::stringstream vShaderStream, fShaderStream;
		/* Reads File's buffer contents into streams */
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		/* Close file handlers */
		vShaderFile.close();
		fShaderFile.close();
		/* Convert File Stream into String */
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	/* Compile Shaders */
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	/* Vertex Shader */
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	/* Print Compile Errors if any occured */
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/* Fragment Shader */
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	/* Print Compile Errors if any occured */
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/* Shader Program */
	/* Sets the ID of the Shader Program */
	ID = glCreateProgram();
	/* Attachers the Shaders to the ID */
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	/* Links the Shader Program ID with OpenGL */
	glLinkProgram(ID);
	/* Print linking errors if any occured */
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	/* Deletes the Shaders after they are linked into our program */
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{

}

void Shader::Bind()
{
	/* Uses the assigned Shader Program */
	GLCall(glUseProgram(ID));
}

void Shader::UnBind()
{
	/* Unbinds the currently bound shader */
	GLCall(glUseProgram(0));
}

void Shader::SetBool(const std::string& a_name, bool a_value) const
{
	GLCall(glUniform1i(glGetUniformLocation(ID, a_name.c_str()), (int)a_value));
}

void Shader::SetInt(const std::string& a_name, int a_value) const
{
	GLCall(glUniform1i(glGetUniformLocation(ID, a_name.c_str()), a_value));
}

void Shader::SetFloat(const std::string& a_name, float a_value) const
{
	GLCall(glUniform1f(glGetUniformLocation(ID, a_name.c_str()), a_value));
}

void Shader::SetUniformMat4F(const std::string& a_name, const glm::mat4& a_matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(a_name), 1, GL_FALSE, &a_matrix[0][0]));
}

void Shader::SetUniform4f(const std::string& a_name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(a_name), v0, v1, v2, v3));
}

void Shader::TerminateProgram()
{
	glDeleteProgram(ID);
}

int Shader::GetUniformLocation(const std::string& a_name)
{
	/* Checks if we already have the Uniform Name listed */
	if (m_UniformLocationCache.find(a_name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[a_name];
	}

	GLCall(int location = glGetUniformLocation(ID, a_name.c_str()));

	if (location == -1)
	{
		std::cout << "Warning Uniform: " << a_name << " Doesnt exist" << std::endl;
	}

	/* Adds the Uniform Location to the list */
	m_UniformLocationCache[a_name] = location;

	return location;
}