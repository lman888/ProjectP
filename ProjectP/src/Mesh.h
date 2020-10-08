#pragma once

#include <string>
#include <vector>

#include "Shader.h"

#include "Extra/stb_image/stb_image.h"

struct Vertex
{
	/* Vertex Buffer Data */
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct MyTexture
{
	/* Texture Data */
	unsigned int m_ID;
	std::string m_type;
	std::string m_path;
};

class Mesh
{
public:

	/* Mesh's Vertex Buffer Data */
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<MyTexture> m_textures;

	Mesh(std::vector<Vertex> a_vertices, std::vector<unsigned int> a_indices, std::vector<MyTexture> a_textures);
	~Mesh() {}

	void Draw(Shader &a_shader);


protected:

private:

	/* ID's of the Buffer Data */
	unsigned int VAO, VBO, IBO;

	void SetupMesh();
};

