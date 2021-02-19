#include "Geometry.h"
#include "TeaPotData.h"

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
	float m_vertices[24 * 5] = 
	{
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
	unsigned int m_indices[12 * 3] =
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

	//InitBuffers(&m_indices, &m_vertices);

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
	/* Slices are how many lines connect the circle from front to back */
	/* Stacks are how many lines are drawn around the circle */

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



	/* Triangle Mesh InitBuffers */
	InitBuffers(&m_el, &m_p, &m_n, &m_tex);
}

void Geometry::GenerateTeaPot(int a_grid, const glm::mat4& a_lidTransform)
{
	int m_verts = 32 * (a_grid + 1) * (a_grid + 1);
	int m_faces = a_grid * a_grid * 32;
	std::vector<GLfloat> m_p(m_verts * 3);
	std::vector<GLfloat> m_n(m_verts * 3);
	std::vector<GLfloat> m_tc(m_verts * 2);
	std::vector<GLuint> m_el(m_faces * 6);

	GeneratePatches(m_p, m_n, m_tc, m_el, a_grid);
	MoveLid(a_grid, m_p, a_lidTransform);

	InitBuffers(&m_el, &m_p, &m_n, &m_tc);
}

void Geometry::GenerateTorus(GLfloat a_outerRadius, GLfloat a_innerRadius, GLuint a_nsides, GLuint a_nRings)
{
	GLuint m_faces = a_nsides * a_nRings;
	/* One Extra Ring to duplicate first ring */
	int m_nVerts = a_nsides * (a_nRings + 1);

	/* Points */
	std::vector<GLfloat> m_p(3 * m_nVerts);
	/* Normals */
	std::vector<GLfloat> m_n(3 * m_nVerts);
	/* Tex Coords */
	std::vector<GLfloat> m_tc(2 * m_nVerts);
	/* Elements */
	std::vector<GLuint> m_el(6 * m_faces);

	/* Generate the Vertex Data */
	float m_ringFactor = glm::two_pi<float>() / a_nRings;
	float m_sideFactor = glm::two_pi<float>() / a_nsides;
	int m_idx = 0, m_tidx = 0;

	for (GLuint m_ring = 0; m_ring <= a_nsides; m_ring++)
	{
		float m_u = m_ring * m_ringFactor;
		float m_cu = cos(m_u);
		float m_su = sin(m_u);

		for (GLuint m_side = 0; m_side < a_nsides; m_side++)
		{
			float m_v = m_side * m_sideFactor;
			float m_cv = cos(m_v);
			float m_sv = sin(m_v);
			float m_r = (a_outerRadius + a_innerRadius * m_cv);

			m_p[m_idx] = m_r * m_cu;
			m_p[m_idx + 1] = m_r * m_su;
			m_p[m_idx + 2] = a_innerRadius * m_sv;
			m_n[m_idx] = m_cv * m_cu * m_r;
			m_n[m_idx + 1] = m_cv * m_su * m_r;
			m_n[m_idx + 2] = m_sv * m_r;
			m_tc[m_tidx] = m_u / glm::two_pi<float>();
			m_tc[m_tidx + 1] = m_v / glm::two_pi<float>();
			m_tidx += 2;

			/* Normalize */
			float m_len = sqrt(m_n[m_idx] * m_n[m_idx] + 
							   m_n[m_idx + 1] * m_n[m_idx + 1] +
							   m_n[m_idx + 2] * m_n[m_idx + 2]);

			m_n[m_idx] /= m_len;
			m_n[m_idx + 1] /= m_len;
			m_n[m_idx + 2] /= m_len;
			m_idx += 3;
		}
	}

	m_idx = 0;
	for (GLuint l_ring = 0; l_ring < a_nRings; l_ring++)
	{
		GLuint m_ringStart = l_ring * a_nsides;
		GLuint m_nextRingStart = (l_ring + 1) * a_nsides;

		for (GLuint l_side = 0; l_side < a_nsides; l_side++)
		{
			int m_nextSide = (l_side + 1) % a_nsides;

			/* The Quad */
			m_el[m_idx] = (m_ringStart + l_side);
			m_el[m_idx + 1] = (m_nextRingStart + l_side);
			m_el[m_idx + 2] = (m_nextRingStart + m_nextSide);
			m_el[m_idx + 3] = m_ringStart + l_side;
			m_el[m_idx + 4] = m_nextRingStart + m_nextSide;
			m_el[m_idx + 5] = (m_ringStart + m_nextSide);
			m_idx += 6;
		}
	}

	InitBuffers(&m_el, &m_p, &m_n, &m_tc);
}

void Geometry::GeneratePatches(std::vector<GLfloat>& a_p, std::vector<GLfloat>& a_n, std::vector<GLfloat>& a_tc, std::vector<GLuint>& a_el, int a_grid)
{
	std::vector<GLfloat> m_B(4 * (a_grid + 1)); /* Pre-Computed Bernstein basis functions */
	std::vector<GLfloat> m_DB(4 * (a_grid + 1)); /* Pre-Computed derivitives of basis functions */

	int m_idx = 0, m_elIndex = 0, m_tcIndex = 0;

	/* Pre-Compute the basis functions (Bernstein Polynomials) */
	/* And their derivatives */
	ComputerBasesFunctions(m_B, m_DB, a_grid);

	/* Build each patch */
	/* The Rim */
	BuildPatchReflect(0, m_B, m_DB, a_p, a_n, a_tc, a_el, m_idx, m_elIndex, m_tcIndex, a_grid, true, true);
	/* The Body */
	BuildPatchReflect(1, m_B, m_DB, a_p, a_n, a_tc, a_el, m_idx, m_elIndex, m_tcIndex, a_grid, true, true);
	BuildPatchReflect(2, m_B, m_DB, a_p, a_n, a_tc, a_el, m_idx, m_elIndex, m_tcIndex, a_grid, true, true);
	/* The Lid */
	BuildPatchReflect(3, m_B, m_DB, a_p, a_n, a_tc, a_el, m_idx, m_elIndex, m_tcIndex, a_grid, true, true);
	BuildPatchReflect(4, m_B, m_DB, a_p, a_n, a_tc, a_el, m_idx, m_elIndex, m_tcIndex, a_grid, true, true);
	/* The Bottom */
	BuildPatchReflect(5, m_B, m_DB, a_p, a_n, a_tc, a_el, m_idx, m_elIndex, m_tcIndex, a_grid, true, true);
	/* The Handle */
	BuildPatchReflect(6, m_B, m_DB, a_p, a_n, a_tc, a_el, m_idx, m_elIndex, m_tcIndex, a_grid, false, true);
	BuildPatchReflect(7, m_B, m_DB, a_p, a_n, a_tc, a_el, m_idx, m_elIndex, m_tcIndex, a_grid, false, true);
	/* The Spout */
	BuildPatchReflect(8, m_B, m_DB, a_p, a_n, a_tc, a_el, m_idx, m_elIndex, m_tcIndex, a_grid, false, true);
	BuildPatchReflect(9, m_B, m_DB, a_p, a_n, a_tc, a_el, m_idx, m_elIndex, m_tcIndex, a_grid, false, true);
}

void Geometry::BuildPatchReflect(int a_patchNum, std::vector<GLfloat>& a_b,
								 std::vector<GLfloat>& a_db, std::vector<GLfloat>& a_v,
								 std::vector<GLfloat>& a_n, std::vector<GLfloat>& a_tc,
								 std::vector<GLuint>& a_el, int& a_index, int& a_elIndex,
								 int& a_tcIndex, int a_grid, bool a_reflectX, bool a_reflectY)
{
	glm::vec3 m_patch[4][4];
	glm::vec3 m_patchRevV[4][4];
	GetPatch(a_patchNum, m_patch, false);
	GetPatch(a_patchNum, m_patchRevV, true);

	// Patch without modification
	BuildPatch(m_patch, a_b, a_db, a_v, a_n, a_tc, a_el,
		a_index, a_elIndex, a_tcIndex, a_grid, glm::mat3(1.0f), true);

	// Patch reflected in x
	if (a_reflectX) {
		BuildPatch(m_patchRevV, a_b, a_db, a_v, a_n, a_tc, a_el,
			a_index, a_elIndex, a_tcIndex, a_grid, glm::mat3(glm::vec3(-1.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f)), false);
	}

	// Patch reflected in y
	if (a_reflectY) {
		BuildPatch(m_patchRevV, a_b, a_db, a_v, a_n, a_tc, a_el,
			a_index, a_elIndex, a_tcIndex, a_grid, glm::mat3(glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f)), false);
	}

	// Patch reflected in x and y
	if (a_reflectX && a_reflectY) {
		BuildPatch(m_patch, a_b, a_db, a_v, a_n, a_tc, a_el,
			a_index, a_elIndex, a_tcIndex, a_grid, glm::mat3(glm::vec3(-1.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f)), true);
	}
}

void Geometry::BuildPatch(glm::vec3 patch[][4],
						  std::vector<GLfloat>& a_B, std::vector<GLfloat>& a_dB,
						  std::vector<GLfloat>& a_v, std::vector<GLfloat>& a_n,
						  std::vector<GLfloat>& a_tc, std::vector<GLuint>& a_el,
						  int& a_index, int& a_elIndex, int& a_tcIndex, int a_grid, glm::mat3 a_reflect,
						  bool a_invertNormal)
{
	int m_startIndex = a_index / 3;
	float m_tcFactor = 1.0f / a_grid;

	for (int i = 0; i <= a_grid; i++)
	{
		for (int j = 0; j <= a_grid; j++)
		{
			glm::vec3 pt = a_reflect * Evaluate(i, j, a_B, patch);
			glm::vec3 norm = a_reflect * EvaluateNormal(i, j, a_B, a_dB, patch);
			if (a_invertNormal)
				norm = -norm;

			a_v[a_index] = pt.x;
			a_v[a_index + 1] = pt.y;
			a_v[a_index + 2] = pt.z;

			a_n[a_index] = norm.x;
			a_n[a_index + 1] = norm.y;
			a_n[a_index + 2] = norm.z;

			a_tc[a_tcIndex] = i * m_tcFactor;
			a_tc[a_tcIndex + 1] = j * m_tcFactor;

			a_index += 3;
			a_tcIndex += 2;
		}
	}

	for (int i = 0; i < a_grid; i++)
	{
		int iStart = i * (a_grid + 1) + m_startIndex;
		int nextiStart = (i + 1) * (a_grid + 1) + m_startIndex;
		for (int j = 0; j < a_grid; j++)
		{
			a_el[a_elIndex] = iStart + j;
			a_el[a_elIndex + 1] = nextiStart + j + 1;
			a_el[a_elIndex + 2] = nextiStart + j;

			a_el[a_elIndex + 3] = iStart + j;
			a_el[a_elIndex + 4] = iStart + j + 1;
			a_el[a_elIndex + 5] = nextiStart + j + 1;

			a_elIndex += 6;
		}
	}
}

void Geometry::GetPatch(int a_patchNum, glm::vec3 a_patch[][4], bool a_reverseV)
{
	for (int u = 0; u < 4; u++) {          // Loop in u direction
		for (int v = 0; v < 4; v++) {     // Loop in v direction
			if (a_reverseV) {
				a_patch[u][v] = glm::vec3(
					TeapotData::cpdata[TeapotData::patchdata[a_patchNum][u * 4 + (3 - v)]][0],
					TeapotData::cpdata[TeapotData::patchdata[a_patchNum][u * 4 + (3 - v)]][1],
					TeapotData::cpdata[TeapotData::patchdata[a_patchNum][u * 4 + (3 - v)]][2]
				);
			}
			else {
				a_patch[u][v] = glm::vec3(
					TeapotData::cpdata[TeapotData::patchdata[a_patchNum][u * 4 + v]][0],
					TeapotData::cpdata[TeapotData::patchdata[a_patchNum][u * 4 + v]][1],
					TeapotData::cpdata[TeapotData::patchdata[a_patchNum][u * 4 + v]][2]
				);
			}
		}
	}
}

void Geometry::ComputerBasesFunctions(std::vector<GLfloat>& a_b, std::vector<GLfloat>& a_db, int a_grid)
{
	float m_inc = 1.0f / a_grid;
	for (int i = 0; i <= a_grid; i++)
	{
		float t = i * m_inc;
		float tSqr = t * t;
		float oneMinusT = (1.0f - t);
		float oneMinusT2 = oneMinusT * oneMinusT;

		a_b[i * 4 + 0] = oneMinusT * oneMinusT2;
		a_b[i * 4 + 1] = 3.0f * oneMinusT2 * t;
		a_b[i * 4 + 2] = 3.0f * oneMinusT * tSqr;
		a_b[i * 4 + 3] = t * tSqr;

		a_db[i * 4 + 0] = -3.0f * oneMinusT2;
		a_db[i * 4 + 1] = -6.0f * t * oneMinusT + 3.0f * oneMinusT2;
		a_db[i * 4 + 2] = -3.0f * tSqr + 6.0f * t * oneMinusT;
		a_db[i * 4 + 3] = 3.0f * tSqr;
	}
}

glm::vec3 Geometry::Evaluate(int a_gridU, int a_gridV, std::vector<GLfloat>& a_b, glm::vec3 a_patch[][4])
{
	glm::vec3 m_p(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m_p += a_patch[i][j] * a_b[a_gridU * 4 + i] * a_b[a_gridV * 4 + j];
		}
	}
	return m_p;
}

glm::vec3 Geometry::EvaluateNormal(int a_gridU, int a_gridV, std::vector<GLfloat>& a_b, std::vector<GLfloat>& a_db, glm::vec3 a_patch[][4])
{
	glm::vec3 m_du(0.0f, 0.0f, 0.0f);
	glm::vec3 m_dv(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m_du += a_patch[i][j] * a_db[a_gridU * 4 + i] * a_b[a_gridV * 4 + j];
			m_dv += a_patch[i][j] * a_b[a_gridU * 4 + i] * a_db[a_gridV * 4 + j];
		}
	}

	glm::vec3 m_norm = glm::cross(m_du, m_dv);
	if (glm::length(m_norm) != 0.0f) {
		m_norm = glm::normalize(m_norm);
	}

	return m_norm;
}

void Geometry::MoveLid(int a_grid, std::vector<GLfloat>& a_p, const glm::mat4& a_lidTransform)
{
	int m_start = 3 * 12 * (a_grid + 1) * (a_grid + 1);
	int m_end = 3 * 20 * (a_grid + 1) * (a_grid + 1);

	for (int i = m_start; i < m_end; i+=3)
	{
		glm::vec4 m_vert = glm::vec4(a_p[i], a_p[i + 1], a_p[i+ 2], 1.0f);
		m_vert = a_lidTransform * m_vert;

		a_p[i] = m_vert.x;
		a_p[i + 1] = m_vert.y;
		a_p[i + 2] = m_vert.z;
	}
}