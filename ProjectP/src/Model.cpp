#include "Model.h"

void Model::DrawModel(Shader& a_shader)
{
    /* Loops over each Mesh to call their draw function */
    for (unsigned int i = 0; i < m_meshes.size(); i++)
        m_meshes[i].Draw(a_shader);
}

void Model::LoadModel(std::string a_path)
{
    Assimp::Importer m_import;
    const aiScene* m_scene = m_import.ReadFile(a_path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!m_scene || m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << m_import.GetErrorString() << std::endl;
        return;
    }

    m_directory = a_path.substr(0, a_path.find_last_of('/'));

    ProcessNode(m_scene->mRootNode, m_scene);
}

void Model::ProcessNode(aiNode* a_node, const aiScene* a_scene)
{
    /* Process all the node's meshes (if any) */
    for (unsigned int i = 0; i < a_node->mNumMeshes; i++)
    {
        aiMesh* m_mesh = a_scene->mMeshes[a_node->mMeshes[i]];
        m_meshes.push_back(ProcessMesh(m_mesh, a_scene));
    }

    /* Then do the same for each of its children */
    for (unsigned int i = 0; i < a_node->mNumChildren; i++)
    {
        ProcessNode(a_node->mChildren[i], a_scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* a_mesh, const aiScene* a_scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<MyTexture> textures;

    for (unsigned int i = 0; i < a_mesh->mNumVertices; i++)
    {
        Vertex vertex;
        /* Process Vertex Position, Normals and Texture Coordinates */

        glm::vec3 vector;
        vector.x = a_mesh->mVertices[i].x;
        vector.y = a_mesh->mVertices[i].y;
        vector.z = a_mesh->mVertices[i].z;

        vertex.Position = vector;

        vector.x = a_mesh->mNormals[i].x;
        vector.y = a_mesh->mNormals[i].y;
        vector.z = a_mesh->mNormals[i].z;

        vertex.Normal = vector;

        if (a_mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = a_mesh->mTextureCoords[0][i].x;
            vec.y = a_mesh->mTextureCoords[0][i].y;

            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);


        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < a_mesh->mNumFaces; i++)
    {
        aiFace face = a_mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (a_mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = a_scene->mMaterials[a_mesh->mMaterialIndex];
        std::vector<MyTexture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<MyTexture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<MyTexture> Model::LoadMaterialTextures(aiMaterial* a_mat, aiTextureType a_type, std::string a_typeName)
{
    std::vector<MyTexture> textures;

    for (unsigned int i = 0; i < a_mat->GetTextureCount(a_type); i++)
    {
        aiString str;
        a_mat->GetTexture(a_type, i, &str);
        bool skip = false;

        for (unsigned int j = 0; j < m_texturesLoaded.size(); j++)
        {
            if (std::strcmp(m_texturesLoaded[j].m_path.data(), str.C_Str()) == 0)
            {
                textures.push_back(m_texturesLoaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            MyTexture texture;
            texture.m_ID = TextureFromFile(str.C_Str(), m_directory);
            texture.m_type = a_typeName;
            texture.m_path = str.C_Str();
            textures.push_back(texture);
            m_texturesLoaded.push_back(texture);
        }

        return textures;
    }

}

unsigned int Model::TextureFromFile(const char* a_path, const std::string& a_directory)
{
    std::string m_fileName = std::string(a_path);

    m_fileName = a_directory + '/' + m_fileName;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;

    unsigned char* data = stbi_load(m_fileName.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture Failed to Load at Path" << a_path << std::endl;
    }

    return textureID;
}
