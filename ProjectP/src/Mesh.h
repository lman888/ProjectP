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
	unsigned int m_id;
	std::string m_type;
	std::string m_path;
};

class Mesh
{
public:

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<MyTexture> m_textures;

	Mesh(std::vector<Vertex> a_vertices, std::vector<unsigned int> a_indices, std::vector<MyTexture> a_textures);
	~Mesh() {}

	void DrawModel(Shader &a_shader);


protected:

private:

	unsigned int VAO, VBO, IBO;

	void SetupMesh();
};

