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

	if (a_source == "")
	{
		std::cout << a_fileName << ": " << " IS::INCORRECT" << std::endl;
	}
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
			std::cout << m_log;
		}
		std::cout << m_msg;
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
			std::cout << m_errString << std::endl;
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

void Shader::BindAttribLocation(unsigned int a_location, const char* a_name)
{
	/* Helper Function, Is the same as stating (location = ) in the Vertex Shader */
	glBindAttribLocation(m_handle, a_location, a_name);
}

void Shader::BindFragDataLocation(unsigned int a_location, const char* a_name)
{
	/* Helper Function, Is the same as stating (location = ) in the Frag Shader */
	glBindFragDataLocation(m_handle, a_location, a_name);
}

void Shader::PrintActiveAttribs()
{
	/* List of Active Vertex Input Attributes and Locations */

	/* Querying for the number of active attributes */
	GLint m_numAttribs;
	glGetProgramInterfaceiv(m_handle, GL_PROGRAM_INPUT,
		                    GL_ACTIVE_RESOURCES, &m_numAttribs);

	/* Loop through each attribute, query for the Length of the name, type and the attribute location */
	GLenum m_properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };
	std::cout << "\nActive Attributes" << std::endl;

	for (int i = 0; i < m_numAttribs; i++)
	{
		GLint m_results[3];
		glGetProgramResourceiv(m_handle, GL_PROGRAM_INPUT, i, 3, m_properties, 3, NULL, m_results);

		GLint m_nameBufSize = m_results[0] + 1;
		char* m_name = new char[m_nameBufSize];
		glGetProgramResourceName(m_handle, GL_PROGRAM_INPUT,
								 i, m_nameBufSize, NULL, m_name);

		printf("%-5d %s (%s)n", m_results[2], m_name, GetValueTypeString(m_results[1]));
		delete[] m_name;
	} std::cout << std::endl;
}

void Shader::PrintActiveUniforms()
{
	/* Will Print out the currently active Uniforms in the Shaders */

	/* Start by querying for the number of active Uniform Variables */
	GLint m_numUniforms = 0;
	glGetProgramInterfaceiv(m_handle, GL_UNIFORM,
							GL_ACTIVE_RESOURCES, &m_numUniforms);

	/* Loop through each Uniform Index and query for the length of the name, type,
	   location and block index */

	GLenum m_properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };

	std::cout << "\nActive Uniforms" << std::endl;
	for (int i = 0; i < m_numUniforms; i++)
	{
		GLint m_result[4];

		glGetProgramResourceiv(m_handle, GL_UNIFORM, i, 4, m_properties, 4, NULL, m_result);

		if (m_result[3] != -1)
			continue; // Skips Uniform Blocks

		GLint m_nameBufSize = m_result[0] + 1;

		char* m_name = new char[m_nameBufSize];

		glGetProgramResourceName(m_handle, GL_UNIFORM, i,
			m_nameBufSize, NULL, m_name);
		printf("%-5d %s (%s)n", m_result[2], m_name, GetValueTypeString(m_result[1]));
		delete[] m_name;
	} std::cout << std::endl;
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

void Shader::SetUniform3f(const std::string& a_name, float v0, float v1, float v2)
{
	GLCall(glUniform3f(GetUniformLocation(a_name), v0, v1, v2));
}

void Shader::SetUniformMat3f(const std::string& a_name, const glm::mat3& a_value)
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

void Shader::SetUniformVec4f(const std::string& a_name, const glm::vec4& a_value)
{
	GLCall(glUniform4fv(GetUniformLocation(a_name), 1, &a_value[0.0f]));
}

void Shader::PracticePipleLine()
{
	/* Loads and Compiles Shaders */
	std::string m_vertCode = LoadShaderAsString("Shaders/Seperable.vert");
	std::string m_fragCode1 = LoadShaderAsString("Shaders/Seperable.frag");
	std::string m_fragCode2 = LoadShaderAsString("Shaders/Seperable1.frag");

	/* Create 3 Shader Programs for each using glCreateShaderProgramV */
	GLuint m_programs[3];
	const GLchar * m_codePtrs[] = { m_vertCode.c_str(), m_fragCode1.c_str(), m_fragCode2.c_str() };
	m_programs[0] = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, m_codePtrs);
	m_programs[1] = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, m_codePtrs + 1);
	m_programs[2] = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, m_codePtrs + 2);

	/* Now we create two pipelines. */
	/* The first will use the Vertex Shader and the first fragment shader, and the second will use the Vertex,
	   shader and second Fragment Shader*/
	GLuint m_pipelines[2];
	glCreateProgramPipelines(2, m_pipelines);
	/* First Pipeline */
	glUseProgramStages(m_pipelines[0], GL_VERTEX_SHADER_BIT, m_programs[0]);
	glUseProgramStages(m_pipelines[0], GL_FRAGMENT_SHADER_BIT, m_programs[1]);
	/* Second Pipeline */
	glUseProgramStages(m_pipelines[1], GL_VERTEX_SHADER_BIT, m_programs[0]);
	glUseProgramStages(m_pipelines[1], GL_FRAGMENT_SHADER_BIT, m_programs[2]);

	GLint m_location = glGetUniformLocation(m_programs[0], "u_MVP");
	glProgramUniform3f(m_programs[0], m_location, 0, 1, 0);

	glUseProgram(0);
	glViewport(0, 0, 1980 / 2, 1080);
	glBindProgramPipeline(m_pipelines[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glUseProgram(0);
	glViewport(1980 / 2, 0, 1980 / 2, 1080);
	glBindProgramPipeline(m_pipelines[1]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);
}

void Shader::UniformBlock()
{
	/* UniformBlock */
	/* Allocate Space for the Buffer to contain Data for the uniform Block */
	unsigned int m_blockIndex = glGetUniformBlockIndex(m_handle, "u_blobSettings");
	int m_blockSize = NULL;
	glGetActiveUniformBlockiv(m_handle, m_blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &m_blockSize);

	GLubyte* m_blockBuffer;
	m_blockBuffer = (GLubyte*)malloc(m_blockSize);

	/* Query for the offset of each variable within the block */
	const char* m_names[] = { "u_blobSettings.innerColor", "u_blobSettings.outerColor",
							  "u_blobSettings.radiusInner", "u_blobSettings.radiusOuter" };
	unsigned int m_indices[4];
	glGetUniformIndices(m_handle, 4, m_names, m_indices);

	int m_offset[4];
	glGetActiveUniformsiv(m_handle, 4, m_indices, GL_UNIFORM_OFFSET, m_offset);

	/* Place Data into the Buffer at the appropriate Offsets */
	float m_outerColor[] = {0.0f, 0.0f, 0.0f, 0.0f };
	float m_innerColor[] = { 1.0f, 1.0f, 0.75f, 1.0f };
	float m_innerRadius = 0.25f, m_outerRadius = 0.45f;

	memcpy(m_blockBuffer + m_offset[0], m_innerColor,
		   4 * sizeof(GLfloat));
	memcpy(m_blockBuffer + m_offset[1], m_outerColor,
		   4 * sizeof(GLfloat));
	memcpy(m_blockBuffer + m_offset[2], &m_innerRadius,
		   sizeof(GLfloat));
	memcpy(m_blockBuffer + m_offset[3], &m_outerRadius,
		   sizeof(GLfloat));

	/* Create the Buffer Object and cope the data into it */
	unsigned int m_uboHandle;
	glGenBuffers(1, &m_uboHandle);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboHandle);
	glBufferData(GL_UNIFORM_BUFFER, m_blockSize, m_blockBuffer,
		         GL_DYNAMIC_DRAW);

	/* Bind the buffer object to the Uniform Buffer-Binding,
	   point at the index specified by the binding-layout qualified in the fragment shader (0) */
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uboHandle);
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
}

void Shader::CreateShaderPrograms()
{
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

const char* Shader::GetValueTypeString(GLenum a_type)
{
	switch (a_type)
	{
	case GL_FLOAT:
		return "float";
	case GL_FLOAT_VEC2:
		return "vec2";
	case GL_FLOAT_VEC3:
		return "vec3";
	case GL_FLOAT_VEC4:
		return "vec4";
	case GL_DOUBLE:
		return "double";
	case GL_INT:
		return "int";
	case GL_UNSIGNED_INT:
		return "unsigned int";
	case GL_BOOL:
		return "bool";
	case GL_FLOAT_MAT2:
		return "mat2";
	case GL_FLOAT_MAT3:
		return "mat3";
	case GL_FLOAT_MAT4:
		return "mat4";
	default:
		return "?";
	}
}