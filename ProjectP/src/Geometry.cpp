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

		//0.5f, -80.5f,  0.0f, 0.0f, // 0 - Bottom Left
		//80.5f, -80.5f,  1.0f, 0.0f, // 1 - Right Side
		//80.5f,  0.5f,  1.0f, 1.0f, // 2 - Top Right
		//0.5f,  0.5f,  0.0f, 1.0f  // 3 - Left Side
	};

	unsigned int m_indices[2 * 3] =
	{
		0, 1, 2, //First Triangle
		2, 3, 0, //Second Triangle

		//4, 5, 6, //First Triangle
		//6, 7, 4, //Second Triangle
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