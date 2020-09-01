#include "Shader.h"

#include "Renderer.h"

namespace GLSLShaderInfo
{
	std::unordered_map<std::string, GLSLShader::GLSLShaderType> m_extensions =
	{
		{ ".vert", GLSLShader::VERTEX},
		{ ".frag", GLSLShader::FRAGMENT }
	};
}

//Shader::Shader(const char* a_vertexPath, const char* a_fragmentPath)
//{
//	/* Retrieves the Vertex/Fragment source code from the File Path */
//	/* Vert Code Path */
//	std::string vertexCode;
//	/* Frag Code Path */
//	std::string fragmentCode;
//	/* Vert Shader File */
//	std::ifstream vShaderFile;
//	/* Frag Shader File */
//	std::ifstream fShaderFile;
//
//	/* Ensure ifstream objects can throw exeptions */
//	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//	try
//	{
//		/* Opens Files */
//		vShaderFile.open(a_vertexPath);
//		fShaderFile.open(a_fragmentPath);
//		/* File Stream holders */
//		std::stringstream vShaderStream, fShaderStream;
//		/* Reads File's buffer contents into streams */
//		vShaderStream << vShaderFile.rdbuf();
//		fShaderStream << fShaderFile.rdbuf();
//		/* Close file handlers */
//		vShaderFile.close();
//		fShaderFile.close();
//		/* Convert File Stream into String */
//		vertexCode = vShaderStream.str();
//		fragmentCode = fShaderStream.str();
//	}
//	catch (std::ifstream::failure e)
//	{
//		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
//	}
//
//	const char* vShaderCode = vertexCode.c_str();
//	const char* fShaderCode = fragmentCode.c_str();
//
//	/* Compile Shaders */
//	unsigned int vertex, fragment;
//	int success;
//	char infoLog[512];
//
//	/* Vertex Shader */
//	vertex = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertex, 1, &vShaderCode, NULL);
//	glCompileShader(vertex);
//	/* Print Compile Errors if any occured */
//	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
//	if (success == GL_FALSE)
//	{
//		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//	}
//
//	/* Fragment Shader */
//	fragment = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragment, 1, &fShaderCode, NULL);
//	glCompileShader(fragment);
//	/* Print Compile Errors if any occured */
//	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
//	if (success == GL_FALSE)
//	{
//		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//	}
//
//	/* Shader Program */
//	/* Sets the ID of the Shader Program */
//	m_ID = glCreateProgram();
//	/* Attachers the Shaders to the ID */
//	glAttachShader(m_ID, vertex);
//	glAttachShader(m_ID, fragment);
//	/* Links the Shader Program ID with OpenGL */
//	glLinkProgram(m_ID);
//	/* Print linking errors if any occured */
//	if (!success)
//	{
//		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//	}
//
//	/* Deletes the Shaders after they are linked into our program */
//	glDeleteShader(vertex);
//	glDeleteShader(fragment);
//}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_handle));
}

void Shader::CompileShader(const char* a_fileName)
{
	/* Checks the File Name's extension to determine the Shader Type */

	std::string m_shaderFile = LoadShaderAsString(a_fileName);
	GLSLShader::GLSLShaderType m_type = GLSLShader::VERTEX;

	auto m_it = GLSLShaderInfo::m_extensions.find(m_shaderFile);
	if (m_it != GLSLShaderInfo::m_extensions.end())
	{
		m_type = m_it->second;
	}
	else
	{
		std::string m_msg = " UNRECOGNIZED::EXTENSION: " + m_shaderFile;
		throw GLSLProgramException(m_msg);
	}

	CompileShader(a_fileName, m_type);
}

void Shader::CompileShader(const char* a_fileName, GLSLShader::GLSLShaderType a_type)
{
	/* Error checks and Converts the file into a string */

	std::ifstream m_inFile(a_fileName, std::ios::in);

	std::stringstream m_shaderCode;
	m_shaderCode << m_inFile.rdbuf();
	m_inFile.close();

	CompileShader(m_shaderCode.str(), a_type, a_fileName);

}

void Shader::CompileShader(const std::string& a_source, GLSLShader::GLSLShaderType a_type, const char* a_fileName)
{
	/* Creates the Shader, Compiles and Shader, Error Checks */
	if (m_handle <= 0)
	{
		m_handle = glCreateProgram();
		if (m_handle == 0)
		{
			throw GLSLProgramException("UNABLE::TO::CREATE::SHADER");
		}
	}

	unsigned int m_shaderHandle = glCreateShader(a_type);

	const char* m_shaderCode = a_source.c_str();
	glShaderSource(m_shaderHandle, 1, &m_shaderCode, NULL);
	glCompileShader(m_shaderHandle);

	int m_result;
	glGetShaderiv(m_shaderHandle, GL_COMPILE_STATUS, &m_result);
	if (GL_FALSE == m_result)
	{
		std::string m_msg;
		if (a_fileName)
		{
			m_msg = std::string(a_fileName) + "SHADER::COMPILATION::FAILED\n";
		}
		else
		{
			m_msg = "SHADER::COMPILATION::FAILED\n";
		}

		int m_length = 0;
		glGetShaderiv(m_shaderHandle, GL_INFO_LOG_LENGTH, &m_length);
		if (m_length > 0)
		{
			std::string m_log(m_length, ' ');
			int m_written = 0;
			glGetShaderInfoLog(m_shaderHandle, m_length, &m_written, &m_log[0]);

			m_msg + m_log;
		}
		throw GLSLProgramException(m_msg);
	}
	else
	{
		glAttachShader(m_handle, m_shaderHandle);
	}

}

void Shader::Bind() const
{
	if (m_handle <= 0 || (!m_linked))
	{
		throw GLSLProgramException("SHADER::IS::NOT::LINKED");
	}
	/* Uses the assigned Shader Program */
	GLCall(glUseProgram(m_handle));
}

void Shader::Link()
{
	if (m_linked)
	{
		return;
	}
	if (m_handle <= 0 )
	{
		throw GLSLProgramException(" PROGRAM::HAS::NOT::BEEN::COMPILED ");
	}

	glLinkProgram(m_handle);

	int m_status = 0;
	std::string m_errString;
	glGetProgramiv(m_handle, GL_LINK_STATUS, &m_status);
	if (GL_FALSE == m_status)
	{
		int m_length = 0;

		glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &m_length);
		m_errString += "PROGRAM::LINK::FAILED\n";

		if (m_length > 0)
		{
			std::string m_log(m_length, ' ');
			int m_written = 0;
			glGetProgramInfoLog(m_handle, m_length, &m_written, &m_log[0]);
			m_errString += m_log;
		}
	}
	else
	{
		//GetUniformLocation();
		m_linked = true;
	}

	DetachAndDeleteShaders();

	if (GL_FALSE == m_status)
	{
		throw GLSLProgramException(m_errString);
	}
}

void Shader::Validate()
{
	if (!m_linked)
	{
		throw GLSLProgramException("PROGRAM::IS::NOT::LINKED");
	}

	int m_status;
	glValidateProgram(m_handle);
	glGetProgramiv(m_handle, GL_VALIDATE_STATUS, &m_status);

	if (GL_FALSE == m_status)
	{
		int m_length = 0;
		std::string m_logString;

		glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &m_length);

		if (m_length > 0)
		{
			char* m_log = new char[m_length];
			int m_written = 0;

			glGetProgramInfoLog(m_handle, m_length, &m_written, m_log);
			m_logString = m_log;
			delete[] m_log;
		}

		throw GLSLProgramException(std::string("PROGRAM::FAILED::TO::VALIDATE\n") + m_logString);

	}
}

void Shader::UnBind() const
{
	/* Unbinds the currently bound shader */
	GLCall(glUseProgram(0));
}

void Shader::Validate() const
{
}

void Shader::BindAttribLocation(unsigned int a_location, const char* a_name)
{
}

void Shader::BindFragDataLocation(unsigned int a_location, const char* a_name)
{
}

void Shader::FindUniformLocations()
{
}

void Shader::SetUniform1f(const std::string& a_name, float a_value)
{
	GLCall(glUniform1f(GetUniformLocation(a_name), a_value));
}

void Shader::SetUniform1i(const std::string& a_name, int a_value)
{
	GLCall(glUniform1i(GetUniformLocation(a_name), a_value));
}

void Shader::SetUniformMat4f(const std::string& a_name, const glm::mat4& a_matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(a_name), 1, GL_FALSE, &a_matrix[0][0]));
}

void Shader::SetUniform4f(const std::string& a_name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(a_name), v0, v1, v2, v3));
}

void Shader::SetUniformMat3f(const std::string& a_name, glm::mat3& a_value)
{
	GLCall(glUniformMatrix3fv(GetUniformLocation(a_name), 1, GL_FALSE, &a_value[0][0]));
}

void Shader::SetUniform1b(const std::string& a_name, bool a_value)
{
	GLCall(glUniform1i(GetUniformLocation(a_name), a_value));
}

void Shader::SetUniformVec3f(const std::string& a_name, const glm::vec3& a_value)
{
	GLCall(glUniform3fv(GetUniformLocation(a_name), 1, &a_value[0.0f]));
}

void Shader::SetUniformVec4f(const std::string& a_name, glm::vec4& a_value)
{
	GLCall(glUniform4fv(GetUniformLocation(a_name), 1, &a_value[0.0f]));
}

void Shader::TerminateProgram()
{
	glDeleteProgram(m_handle);
}

int Shader::GetUniformLocation(const std::string& a_name)
{
	/* Checks if we already have the Uniform Name listed */
	if (m_UniformLocationCache.find(a_name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[a_name];
	}

	GLCall(int location = glGetUniformLocation(m_handle, a_name.c_str()));

	if (location == -1)
	{
		std::cout << "Warning Uniform: " << a_name << " Doesnt exist" << std::endl;
	}

	/* Adds the Uniform Location to the list */
	m_UniformLocationCache[a_name] = location;

	return location;
}

std::string Shader::LoadShaderAsString(const char* a_fileName)
{

	std::string m_nameStr(a_fileName);

	size_t m_dotLoc = m_nameStr.find_last_of('.');

	if (m_dotLoc != std::string::npos)
	{
		std::string m_ext = m_nameStr.substr(m_dotLoc);
		if (m_ext == ".glsl")
		{
			size_t m_loc = m_nameStr.find_last_of('.', m_dotLoc - 1);
			if (m_loc == std::string::npos)
			{
				m_loc = m_nameStr.find_last_of('_', m_dotLoc - 1);
			}
			if (m_loc != std::string::npos)
			{
				return m_nameStr.substr(m_loc);
			}
		}
		else
		{
			return m_ext;
		}
	}
	return "";

	///* Shader Code Path */
	//std::string m_shaderCode;
	///* Shader File */
	//std::ifstream m_shaderFile;

	///* Ensures ifstream objects can throw exceptions */
	//m_shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	//try
	//{
	//	m_shaderFile.open(a_fileName);
	//	/* File Stream Holders */
	//	std::stringstream m_shaderStream;
	//	/* Reads the File's buffer contents into the stream */
	//	m_shaderStream << m_shaderFile.rdbuf();
	//	m_shaderFile.close();

	//	m_shaderCode = m_shaderStream.str();
	//}
	//catch(std::ifstream::failure e)
	//{
	//	std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	//}

	//return m_shaderCode;
}

void Shader::DetachAndDeleteShaders()
{
	int m_numOfShaders = 0;
	glGetProgramiv(m_handle, GL_ATTACHED_SHADERS, &m_numOfShaders);
	std::vector<unsigned int> m_shaderNames(m_numOfShaders);

	glGetAttachedShaders(m_handle, m_numOfShaders, NULL, m_shaderNames.data());
	for ( unsigned int m_shader : m_shaderNames )
	{
		glDetachShader(m_handle, m_shader);
		glDeleteShader(m_shader);
	}
}