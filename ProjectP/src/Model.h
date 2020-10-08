#pragma once

#include <vector>
#include <string>

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:

	Model(const char* a_path)
	{
		LoadModel(a_path);
	}
	~Model() {}

	void DrawModel(Shader& a_shader);

protected:

private:

	std::vector<Mesh> m_meshes;
	std::vector<MyTexture> m_texturesLoaded;
	std::string m_directory;

	void LoadModel(std::string a_path);
	void ProcessNode(aiNode *a_node, const aiScene *a_scene);
	Mesh ProcessMesh(aiMesh *a_mesh, const aiScene *a_scene);
	std::vector<MyTexture> LoadMaterialTextures(aiMaterial *a_mat, aiTextureType a_type, std::string a_typeName);
	unsigned int TextureFromFile(const char* a_path, const std::string& a_directory);
};