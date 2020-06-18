#include "Renderer.h"

void Renderer::DrawTriangle()
{
	glBindVertexArray(VAO);

	/* This function draws primitives using the current active shader */
	glDrawArrays(GL_TRIANGLES, 0, 3);

	/* Draws Triagnles in WireFrame Mode */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	/* Sets the Triangles to its default look */
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::GenerateTriangle()
{
	/* Draws the Triangle */

    /* The Triangle primitive Coordinates */
	float m_vertices[18] =
	{
		/* First Triangle */  /* Colors */
		0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,
	   -0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.0f,  0.5f, 0.0f,    0.0f, 0.0f,1.0f
	};

	/* Generates a Buffer ID using the glGenBuffers (VBO - Vertex Buffer Object) */
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	/* Binds the VAO (Vertex Array Object) From this point onwards, we can bind/configure the - */
	/* Corresponding VBO(s) and attribute pointer(s) and then unbind the VAO for later use */
	glBindVertexArray(VAO);
	/* GL_ARRAY_BUFFER is a Vertex Buffer Object */
	/* From this point, any buffer calls we make (on GL_ARRAY_BUFFER) will be used to configure - */
	/* The current bound Buffer Object (Which is VBO) */
	/* glBindBuffer binds the newly created Buffer to the Buffer Object (GL_ARRAY_BUFFER) */
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	/* glBufferData function copies the Vertex Data into the Buffers memory */
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

	/* Sets the Vertex Attribute Pointers */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* Color Attribute */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Renderer::Terminate()
{
	/* Deletes/de-allocates all resources once they've outlived their purpose */
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
