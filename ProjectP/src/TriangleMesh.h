#pragma once

#include <vector>

#include "GLAD/glad.h"
#include "Drawable.h"

class TriangleMesh : Drawable
{
public:

	virtual ~TriangleMesh();
	/* Overriding Drawable Render*/
	virtual void Render() const override;
	/* Returns the VAO */
	GLuint GetVAO() { return m_vao; };
	/* Returns the position of the Element Buffer */
	GLuint GetElementBuffer() { return m_buffers[0]; };
	/* Returns the position of the Positional Buffer */
	GLuint GetPositionBuffer() { return m_buffers[1]; };
	/* Returns the position of the Normal Buffer */
	GLuint GetNormalBuffer() { return m_buffers[2]; };
	/* Returns the Tex Coords Buffer */
	GLuint GetTCBuffer() { if (m_buffers.size() > 3) return m_buffers[3]; else return 0; };
	/* Obtains the number of Vertices */
	GLuint GetNumVerts() { return m_nVerts; };

protected:

	/* Number of Vertices */
	GLuint m_nVerts;
	/* Vertex Array Object */
	GLuint m_vao;
	/* Vertex Buffers */
	std::vector<GLuint> m_buffers;

	/* Initialises the Object Buffers */
	virtual void InitBuffers
	(
		std::vector<GLuint>* a_indices,
		std::vector<GLfloat>* a_points,
		std::vector<GLfloat>* a_normals,
		std::vector<GLfloat>* a_texCoords = nullptr,
		std::vector<GLfloat>* a_tangents = nullptr
	);

	///* Deletes the Object Buffers */
	virtual void DeleteBuffers();
};