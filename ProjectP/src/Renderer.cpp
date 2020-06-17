#include "Renderer.h"

void Renderer::DrawTriangle()
{

	/* Draws the Triangle */

	/* The Triangle primitive Coordinates */
	float m_vertices[18] =
	{
		/* First Triangle */
		0.5f,  0.5f, 0.0f, /* Top Right */
		0.5f, -0.5f, 0.0f, /* Bottom Right */
	   -0.5f, -0.5f, 0.0f, /* Bottom Left */
	   -0.5f,  0.5f, 0.0f  /* Top Left */
	};

	unsigned int m_indices[6]
	{
		0, 1, 3, /* First Triangle */
		1, 2, 3  /* Second Triangle */
	};

	/* Generates a Buffer ID using the glGenBuffers (VBO - Vertex Buffer Object) */
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);

	/* Binds the VAO (Vertex Array Object) From this point onwards, we can bind/configure the - */
	/* Corresponding VBO(s) and attribute pointer(s) and then unbind the VAO for later use */
	glBindVertexArray(VAO);
	/* GL_ARRAY_BUFFER is a Vertex Buffer Object */
	/* glBindBuffer binds the newly created Buffer to the Buffer Object (GL_ARRAY_BUFFER) */
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

	/* From this point, any buffer calls we make (on GL_ARRAY_BUFFER) will be used to configure - */
	/* The current bound Buffer Object (Which is VBO) */
	/* glBufferData function copies the Vertex Data into the Buffers memory */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

	/* Sets the Vertex Attribute Pointers */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* Uses the newly created Shader Program */
	/* Every Shader and rendering call after glUseProgram will now use this Program Object (and this the Shaders) */
	glUseProgram(m_shaderProgram);

	glBindVertexArray(VAO);
	/* This function draws primitives using the current active shader */
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	/* Draws Triagnles in WireFrame Mode */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	/* Sets the Triangles to its default look */
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);
}

void Renderer::CompileShader()
{
	/* Compiles the basic Vertex Shader Code */

	/* Stores the Vertex Shader into the Shader ID */
	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);

	/* Attach the Vertex Shader source code to the Vertex Shader object */
	glShaderSource(m_vertexShader, 1, &m_vertexShaderSource, nullptr);

	/* Compile the Vertex Shader */
	glCompileShader(m_vertexShader);

	/* This checks if compilation was successfull */
	glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &m_success);

	if (!m_success)
	{
		/* Retrieves the error messages */
		glGetShaderInfoLog(m_vertexShader, 512, nullptr, m_infoLog);
		/* Prints out the error message */
		std::cout << " ERROR::SHADER::VERTEX::COMPILATION_FAILED\n " << m_infoLog << std::endl;
	}

	/* Compiltes the basic Fragment Shader Code */

	/* Stores the Fragment Shader into the Shader ID */
	m_fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	/* Attach the Fragment Shader source code to the Fragment Shader object */
	glShaderSource(m_fragShader, 1, &m_fragShaderSource, nullptr);

	/* Compiles the Fragmant Shader */
	glCompileShader(m_fragShader);

	/* This checks if the compilation was successfull */
	glGetShaderiv(m_fragShader, GL_COMPILE_STATUS, &m_success);

	if (!m_success)
	{
		/* Retrieves the error message */
		glGetShaderInfoLog(m_fragShader, 512, nullptr, m_infoLog);
		/* Prints out the error message */
		std::cout << " ERROR::SHADER::FRAGMENT::COMPILARION_FAILED\n " << m_infoLog << std::endl;
	}

	/* Shader Program */

	/* Final linked version of the Shaders combined */
	/* Returns the ID of the newly created Shader Program Object (m_shaderProgram) */
	m_shaderProgram = glCreateProgram();

	/* Attach and Linking stage */
	/* Attach's the Vertex Shader to the Shader Program */
	glAttachShader(m_shaderProgram, m_vertexShader);
	/* Attach's the Fragment Shader to the Shader Program */
	glAttachShader(m_shaderProgram, m_fragShader);
	/* Links the Shader Program */
	glLinkProgram(m_shaderProgram);

	/* Checks if Shader Program failed the Linking */
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &m_success);
	if (!m_success)
	{
		/* Retrieves the error message */
		glGetProgramInfoLog(m_shaderProgram, 512, nullptr, m_infoLog);
		/* Prints out the error message */
		std::cout << " ERROR::SHADER::PROGRAM:COMPILATION_FAILED\n " << m_infoLog << std::endl;
	}

	/* Don't forget to delete the Shader Objects once we've linked them into the program */
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragShader);
}