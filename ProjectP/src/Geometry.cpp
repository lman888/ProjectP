#include "Geometry.h"

Geometry::Geometry()
{
}

Geometry::~Geometry()
{
}

void Geometry::GenerateQuad()
{
	/* Vertex Positions for Quad */
	float m_vertices[4 * 4] =
	{
		//Position       //Texture Coords
	   -80.5f, -80.5f,  0.0f, 0.0f, // 0 - Bottom Left
	   -0.5f,  -80.5f,  1.0f, 0.0f, // 1 - Right Side
	   -0.5f,   0.5f,   1.0f, 1.0f, // 2 - Top Right
	   -80.5f,  0.5f,   0.0f, 1.0f,  // 3 - Left Side
	};

	unsigned int m_indices[2 * 3] =
	{
		0, 1, 2, //First Triangle
		2, 3, 0, //Second Triangle
	};

	VertexBuffer VB(m_vertices, 4 * 4 * sizeof(float));
	VertexBufferLayout Layout;

	Layout.Push<float>(2);
	Layout.Push<float>(2);

	VertexArray VA;

	VA.AddBuffer(VB, Layout);

	IndexBuffer IB(m_indices, 2 * 3);

	VA.UnBind();
	VB.UnBind();
	IB.UnBind();

	Renderer m_renderer;
	m_renderer.Draw(VA, IB);
}

void Geometry::GenerateCube()
{
	/* Vertex Positions for Cube */
	float m_vertices[24 * 5] = {
		 80.0f, -80.0f, -80.0f,  0.0f, 0.0f, // 0
		-80.0f, -80.0f, -80.0f,  1.0f, 0.0f, // 1
		-80.0f,  80.0f, -80.0f,  1.0f, 1.0f, // 2
		 80.0f,  80.0f, -80.0f,  0.0f, 1.0f, // 3

		 80.0f, -80.0f,  80.0f,  0.0f, 0.0f, // 4
		-80.0f, -80.0f,  80.0f,  1.0f, 0.0f, // 5
		-80.0f,  80.0f,  80.0f,  1.0f, 1.0f, // 6
		 80.0f,  80.0f,  80.0f,  0.0f, 1.0f, // 7

		 80.0f,  80.0f,  80.0f,  1.0f, 0.0f, // 8
		 80.0f,  80.0f, -80.0f,  1.0f, 1.0f, // 9
		 80.0f, -80.0f, -80.0f,  0.0f, 1.0f, // 10
		 80.0f, -80.0f,  80.0f,  0.0f, 0.0f, // 11

		-80.0f,  80.0f,  80.0f,  1.0f, 0.0f, // 12
		-80.0f,  80.0f, -80.0f,  1.0f, 1.0f, // 13
		-80.0f, -80.0f, -80.0f,  0.0f, 1.0f, // 14
		-80.0f, -80.0f,  80.0f,  0.0f, 0.0f, // 15

		 80.0f, -80.0f, -80.0f,  0.0f, 1.0f, // 16
		-80.0f, -80.0f, -80.0f,  1.0f, 1.0f, // 17
		-80.0f, -80.0f,  80.0f,  1.0f, 0.0f, // 18
		 80.0f, -80.0f,  80.0f,  0.0f, 0.0f, // 19

		 80.0f,  80.0f, -80.0f,  0.0f, 1.0f, // 20
		-80.0f,  80.0f, -80.0f,  1.0f, 1.0f, // 21
		-80.0f,  80.0f,  80.0f,  1.0f, 0.0f, // 22
		 80.0f,  80.0f,  80.0f,  0.0f, 0.0f, // 23
	};

	/* Indices for Cube */
	unsigned int m_indices[12 * 3]= 
	{
		0, 1, 2, //First Triangle
		2, 3, 0, //Second Triangle

		4, 5, 6, //First Triangle
		6, 7, 4, //Second Triangle

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20,
	};

	VertexBuffer VB(m_vertices, 24 * 5 * sizeof(float));
	VertexBufferLayout Layout;

	Layout.Push<float>(3);
	Layout.Push<float>(2);
	
	VertexArray VA;

	VA.AddBuffer(VB, Layout);

	IndexBuffer IB(m_indices, 12 * 3);

	VA.UnBind();
	VB.UnBind();
	IB.UnBind();

	Renderer m_renderer;
	m_renderer.Draw(VA, IB);
}

void Geometry::GenerateSphere(float a_radius, unsigned int a_nSlices, unsigned int a_nStacks)
{
	int m_nVerts = (a_nSlices + 1) * (a_nStacks + 1);
	int m_elements = (a_nSlices * 2 * (a_nStacks - 1)) * 3;

	/* Vertecies */
	std::vector<float> m_p(3 * m_nVerts);
	/* Normals */
	std::vector<float> m_n(3 * m_nVerts);
	/* Tex Coords */
	std::vector<float> m_tex(2 * m_nVerts);
	/* Elements */
	std::vector<unsigned int> m_el(m_elements);

	// Generate positions and normals
	GLfloat theta, phi;
	GLfloat thetaFac = glm::two_pi<float>() / a_nSlices;
	GLfloat phiFac = glm::pi<float>() / a_nStacks;
	GLfloat nx, ny, nz, s, t;
	GLuint idx = 0, tIdx = 0;
	for (GLuint i = 0; i <= a_nSlices; i++) {
		theta = i * thetaFac;
		s = (GLfloat)i / a_nSlices;
		for (GLuint j = 0; j <= a_nStacks; j++) {
			phi = j * phiFac;
			t = (GLfloat)j / a_nStacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);
			m_p[idx] = a_radius * nx; m_p[idx + 1] = a_radius * ny; m_p[idx + 2] = a_radius * nz;
			m_n[idx] = nx; m_n[idx + 1] = ny; m_n[idx + 2] = nz;
			idx += 3;

			m_tex[tIdx] = s;
			m_tex[tIdx + 1] = t;
			tIdx += 2;
		}
	}

	// Generate the element list
	idx = 0;
	for (GLuint i = 0; i < a_nSlices; i++) {
		GLuint stackStart = i * (a_nStacks + 1);
		GLuint nextStackStart = (i + 1) * (a_nStacks + 1);
		for (GLuint j = 0; j < a_nStacks; j++) {
			if (j == 0) {
				m_el[idx] = stackStart;
				m_el[idx + 1] = stackStart + 1;
				m_el[idx + 2] = nextStackStart + 1;
				idx += 3;
			}
			else if (j == a_nStacks - 1) {
				m_el[idx] = stackStart + j;
				m_el[idx + 1] = stackStart + j + 1;
				m_el[idx + 2] = nextStackStart + j;
				idx += 3;
			}
			else {
				m_el[idx] = stackStart + j;
				m_el[idx + 1] = stackStart + j + 1;
				m_el[idx + 2] = nextStackStart + j + 1;
				m_el[idx + 3] = nextStackStart + j;
				m_el[idx + 4] = stackStart + j;
				m_el[idx + 5] = nextStackStart + j + 1;
				idx += 6;
			}
		}
	}

	InitSphere(&m_el, &m_p, &m_n, &m_tex);
}

void Geometry::InitSphere(std::vector<GLuint>* a_indices,
						  std::vector<GLfloat>* a_points,
						  std::vector<GLfloat>* a_normals,
						  std::vector<GLfloat>* a_texCoords,
						  std::vector<GLfloat>* a_tangents)
{

	// Must have data for indices, points, and normals
	if (a_indices == nullptr || a_points == nullptr || a_normals == nullptr)
		return;

	m_sphereVerts = (GLuint)a_indices->size();

	GLuint indexBuf = 0, posBuf = 0, normBuf = 0, tcBuf = 0, tangentBuf = 0;
	glGenBuffers(1, &indexBuf);
	m_spherebuffers.push_back(indexBuf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_indices->size() * sizeof(GLuint), a_indices->data(), GL_STATIC_DRAW);

	glGenBuffers(1, &posBuf);
	m_spherebuffers.push_back(posBuf);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf);
	glBufferData(GL_ARRAY_BUFFER, a_points->size() * sizeof(GLfloat), a_points->data(), GL_STATIC_DRAW);

	glGenBuffers(1, &normBuf);
	m_spherebuffers.push_back(normBuf);
	glBindBuffer(GL_ARRAY_BUFFER, normBuf);
	glBufferData(GL_ARRAY_BUFFER, a_normals->size() * sizeof(GLfloat), a_normals->data(), GL_STATIC_DRAW);

	if (a_texCoords != nullptr) {
		glGenBuffers(1, &tcBuf);
		m_spherebuffers.push_back(tcBuf);
		glBindBuffer(GL_ARRAY_BUFFER, tcBuf);
		glBufferData(GL_ARRAY_BUFFER, a_texCoords->size() * sizeof(GLfloat), a_texCoords->data(), GL_STATIC_DRAW);
	}

	if (a_tangents != nullptr) {
		glGenBuffers(1, &tangentBuf);
		m_spherebuffers.push_back(tangentBuf);
		glBindBuffer(GL_ARRAY_BUFFER, tangentBuf);
		glBufferData(GL_ARRAY_BUFFER, a_tangents->size() * sizeof(GLfloat), a_tangents->data(), GL_STATIC_DRAW);
	}

	glGenVertexArrays(1, &m_sphereVao);
	glBindVertexArray(m_sphereVao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuf);

	// Position
	glBindBuffer(GL_ARRAY_BUFFER, posBuf);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	// Normal
	glBindBuffer(GL_ARRAY_BUFFER, normBuf);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Normal

	// Tex coords
	if (a_texCoords != nullptr) {
		glBindBuffer(GL_ARRAY_BUFFER, tcBuf);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);  // Tex coord
	}

	if (a_tangents != nullptr) {
		glBindBuffer(GL_ARRAY_BUFFER, tangentBuf);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(3);  // Tangents
	}

	glBindVertexArray(0);

	DrawSphere();
}

void Geometry::DrawSphere()
{
	glBindVertexArray(m_sphereVao);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawElements(GL_TRIANGLES, m_sphereVerts, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}