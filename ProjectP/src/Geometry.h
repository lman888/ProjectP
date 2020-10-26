#pragma once


#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Geometry
{
public:

	Geometry();
	~Geometry();

	void GenerateQuad();

	void GenerateCube();

	void GenerateSphere(float a_radius, unsigned int a_nSlices, unsigned int a_nStacks);

protected:

private:

	void InitSphere(std::vector<GLuint>* a_indices,
		std::vector<GLfloat>* a_points,
		std::vector<GLfloat>* a_normals,
		std::vector<GLfloat>* a_texCoords = nullptr,
		std::vector<GLfloat>* a_tangents = nullptr);

	void DrawSphere();

	std::vector<GLuint> m_spherebuffers;
	GLuint m_sphereVerts;     // Number of vertices
	GLuint m_sphereVao;        // The Vertex Array Object
};

