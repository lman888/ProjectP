#include "TriangleMesh.h"

void TriangleMesh::InitBuffers(std::vector<GLuint>* a_indices, std::vector<GLfloat>* a_points, std::vector<GLfloat>* a_normals, std::vector<GLfloat>* a_texCoords, std::vector<GLfloat>* a_tangents)
{
	/* If the buffers arent empty, delete the data inside */
	if (!m_buffers.empty())
	{
		DeleteBuffers();
	}

	/* Check if there is data in the buffers */
	if (a_indices == nullptr || a_normals == nullptr || a_points == nullptr)
	{
		return;
	}

	/* Number of Vertices */
	m_nVerts = (GLuint)a_indices->size();

	/* Buffers */
	GLuint m_indexBuf = 0, m_posBuf = 0, m_normBuf = 0, m_tcBuf = 0, m_tangentBuf = 0;
	/* Generates and Fills the Index Buffer */
	glGenBuffers(1, &m_indexBuf);
	m_buffers.push_back(m_indexBuf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_indices->size() * sizeof(GLuint), a_indices->data(), GL_STATIC_DRAW);

	/* Generates and Fills the Position Buffer */
	glGenBuffers(1, &m_posBuf);
	m_buffers.push_back(m_posBuf);
	glBindBuffer(GL_ARRAY_BUFFER, m_posBuf);
	glBufferData(GL_ARRAY_BUFFER, a_points->size() * sizeof(GLfloat), a_points->data(), GL_STATIC_DRAW);

	/* Generates and Fills the Normal Buffer */
	glGenBuffers(1, &m_normBuf);
	m_buffers.push_back(m_normBuf);
	glBindBuffer(GL_ARRAY_BUFFER, m_normBuf);
	glBufferData(GL_ARRAY_BUFFER, a_normals->size() * sizeof(GLfloat), a_normals->data(), GL_STATIC_DRAW);

	if (a_texCoords != nullptr)
	{
		glGenBuffers(1, &m_tcBuf);
		m_buffers.push_back(m_tcBuf);
		glBindBuffer(GL_ARRAY_BUFFER, m_tcBuf);
		glBufferData(GL_ARRAY_BUFFER, a_texCoords->size() * sizeof(GLfloat), a_texCoords->data(), GL_STATIC_DRAW);
	}

	if (a_tangents != nullptr)
	{
		glGenBuffers(1, &m_tangentBuf);
		m_buffers.push_back(m_tangentBuf);
		glBindBuffer(GL_ARRAY_BUFFER, m_tangentBuf);
		glBufferData(GL_ARRAY_BUFFER, a_tangents->size() * sizeof(GLfloat), a_tangents->data(), GL_STATIC_DRAW);
	}

	/* Generate the Vertex Array Buffer and Bind */
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	/* Binds the Index Buffer */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuf);

	/* Position */
	glBindBuffer(GL_ARRAY_BUFFER, m_posBuf);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0); /* Position */
	
	/* Normal */
	glBindBuffer(GL_ARRAY_BUFFER, m_normBuf);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1); /* Normal */

	/* Tex Coords */
	if (a_texCoords != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_tcBuf);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
	}

	/* Tangents */
	if (a_tangents != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_tangentBuf);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(3);
	}

 	Render();
}

void TriangleMesh::Render() const
{
	if (m_vao == 0)
		return;

	/* Binds the Vertex Array */
	glBindVertexArray(m_vao);
	/* Draws the object with the Data provided */
	glDrawElements(GL_TRIANGLES, m_nVerts, GL_UNSIGNED_INT, 0);
	/* UnBinds the Vertex Array */
	glBindVertexArray(0);
}

TriangleMesh::~TriangleMesh()
{
	DeleteBuffers();
}

void TriangleMesh::DeleteBuffers()
{
	/* If the buffer is filled, delete it */
	if (m_buffers.size() > 0)
	{
		glDeleteBuffers((GLsizei)m_buffers.size(), m_buffers.data());
	}

	/* If the VAO exists, delete it */
	if (m_vao != 0)
	{
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
}