#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> a_vertices, std::vector<unsigned int> a_indices, std::vector<MyTexture> a_textures)
{
	/* Sets the required Vertices, Indices and Textures */
	this->m_vertices = a_vertices;
	this->m_indices = a_indices;
	//this->m_textures = a_textures;

	SetupMesh();
}

void Mesh::DrawModel(Shader& a_shader)
{
	unsigned int m_diffuseNr = 1;
	unsigned int m_specularNr = 1;

	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		/* Activates the Texture before binding */
		glActiveTexture(GL_TEXTURE + i);
		/* retrieves texture number */
		std::string m_number;
		std::string m_name = m_textures[i].m_type;
		if (m_name == "texture_diffuse")
		{
			m_number = std::to_string(m_diffuseNr++);
		}
		else if (m_name == "texture_specular")
		{
			m_number = std::to_string(m_specularNr++);
		}
		a_shader.SetUniform1f(("material." + m_name + m_number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
	}
	glActiveTexture(GL_TEXTURE0);

	/* Draw Mesh */
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::SetupMesh()
{
	/* Generate the Buffers */
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	/* Binds the Buffers to the GPU */
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	/* Vertex Positions */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	/* Vertex Normals */
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	/* Vertex Texture Coords */
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	/* Unbinds the Buffer */
	glBindVertexArray(0);
}