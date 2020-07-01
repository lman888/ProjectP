#include "Renderer.h"

void GLClearError()
{
	if (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* a_function, const char* a_file, int a_line)
{
	if (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << a_function <<
			" " << a_file << ":" << a_line << std::endl;
		return false;
	}
	return true;
}

void Renderer::DrawTriangle()
{
	///* This function draws primitives using the current active shader */
	//glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	///* Draws Triagnles in WireFrame Mode */
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	///* Sets the Triangles to its default look */
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::GenerateTriangle()
{
	/* Draws the Triangle */

	/* Rectangle Primitives */
	//float m_vertices[36] =
	//{
	//	//Position           //Colors             //Texture Coords
	//	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,    1.0f, 1.0f,	//Top Right
	//	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.0f,	//Bottom Right
	//   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,	//Bottom Left
	//   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,    0.0f, 1.0f	//Top Left
	//};

	//unsigned int m_indices[6] =
	//{
	//	0, 1, 3, //First Triangle
	//	1, 2, 3  //Second Triangle
	//};

	///* Generates a Buffer ID using the glGenBuffers (VBO - Vertex Buffer Object) */
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);
	//glGenBuffers(1, &IBO);

	///* Binds the VAO (Vertex Array Object) From this point onwards, we can bind/configure the - */
	///* Corresponding VBO(s) and attribute pointer(s) and then unbind the VAO for later use */
	//glBindVertexArray(VAO);
	///* GL_ARRAY_BUFFER is a Vertex Buffer Object */
	///* From this point, any buffer calls we make (on GL_ARRAY_BUFFER) will be used to configure - */
	///* The current bound Buffer Object (Which is VBO) */
	///* glBindBuffer binds the newly created Buffer to the Buffer Object (GL_ARRAY_BUFFER) */
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	///* glBufferData function copies the Vertex Data into the Buffers memory */
	//glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

	///* Sets the Vertex Attribute Pointers */
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	///* Color Attribute */
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	///* Texture Coord Attribute */
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);
}


void Renderer::Terminate()
{
	/* Deletes/de-allocates all resources once they've outlived their purpose */
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &IBO);
}
