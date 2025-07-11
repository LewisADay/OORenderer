#include "Model.h"

#include <iostream>
#include <ranges>
#include <algorithm>

namespace OORenderer {

	// Init static member variables
	std::vector<std::shared_ptr<Texture>> Model::sm_LoadedTextures = {};

	Model::Model(std::filesystem::path path) {
		LoadFromPath(path);
	}

	void Model::Render(ShaderProgram& shader) {
		for (const auto& mesh : m_Meshes) {
			mesh.Render(shader);
		}
	}

	void Model::RegisterOnGLFWWindow(GLFWwindow* window) {

		// Register textures on correct context
		for (auto& texture : sm_LoadedTextures) {
			texture->BindToWindow(window);
		}
		
		// Register mesh data on correct window
		for (auto& mesh : m_Meshes) {
			mesh.RegisterOnGLFWWindow(window);
		}
	}

	void Model::RegisterOnWindow(const Window& window) {
		RegisterOnGLFWWindow(window.GetGLFWWindow());
	}

	void Model::LoadFromPath(std::filesystem::path path) {
		Assimp::Importer import;

		const aiScene* scene = import.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}
		m_ModelDirectory = path.parent_path();

		ProcessASSIMPNode(scene->mRootNode, scene);
	}

	void Model::ProcessASSIMPNode(aiNode* node, const aiScene* scene) {
		
		for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			ProcessASSIMPMesh(mesh, scene);
		}

		//  Cover the children
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			ProcessASSIMPNode(node->mChildren[i], scene);
		}
	}

	void Model::ProcessASSIMPMesh(aiMesh* mesh, const aiScene* scene) {
		
		// Get vertex data
		std::vector<Mesh::Vertex> vertices;
		vertices.reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			vertices.emplace_back(
				glm::vec3{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z },
				glm::vec3{ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z },
				glm::vec2{ mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][i].x : 0.0f, mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][i].y : 0.0f });
		}

		// Get index data
		std::vector<unsigned int> indices;
		indices.reserve(mesh->mNumFaces * mesh->mFaces[0].mNumIndices); // Likely to be the correct size
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		// Get material data 
		// Build name-shader pair set
		std::map<std::string, std::shared_ptr<Texture>> textureBindingMap;
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			BuildTextureBindingMapFromMaterial(textureBindingMap, material, aiTextureType_DIFFUSE, "DiffuseTexture");
			BuildTextureBindingMapFromMaterial(textureBindingMap, material, aiTextureType_SPECULAR, "SpecularTexture");
			BuildTextureBindingMapFromMaterial(textureBindingMap, material, aiTextureType_AMBIENT, "AmbientTexture");
			BuildTextureBindingMapFromMaterial(textureBindingMap, material, aiTextureType_NORMALS, "NormalTexture");
			BuildTextureBindingMapFromMaterial(textureBindingMap, material, aiTextureType_HEIGHT, "HeightTexture");
		}

		m_Meshes.emplace_back(vertices, indices, textureBindingMap);
	}

	std::vector<std::shared_ptr<Texture>> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type) const {
		
		std::vector<std::shared_ptr<Texture>> textures;
		textures.reserve(material->GetTextureCount(type));

		for (int i = 0; i < material->GetTextureCount(type); ++i) {
			aiString texturePathAIStr;
			material->GetTexture(type, i, &texturePathAIStr);

			// Get full path to texture
			std::filesystem::path texturePath{ texturePathAIStr.C_Str() };
			if (texturePath.is_relative()) {
				texturePath = m_ModelDirectory / texturePath;
			}

			if (!std::filesystem::exists(texturePath)) {
				// TODO LOGGING
				std::cerr << "Failure to load texture at path " << texturePath << ". File not found." << std::endl;
				continue;
			}

			auto alreadyLoadedIterator = std::ranges::find_if(sm_LoadedTextures,
				[&texturePath](std::shared_ptr<Texture> texture) -> bool {
					return texture->GetTexturePath() == texturePath;
				});

			if (alreadyLoadedIterator != sm_LoadedTextures.end()) { // Texture already loaded, so just copy the pointer to that object
				textures.push_back(*alreadyLoadedIterator);
				continue;
			}

			 // Load and keep track of the texture
			auto newlyLoadedTexture = std::make_shared<Texture>(texturePath);
			sm_LoadedTextures.push_back(newlyLoadedTexture);
			textures.push_back(newlyLoadedTexture);
		}

		textures.resize(textures.size());
		return textures;
	}

	void Model::BuildTextureBindingMapFromMaterial(
		std::map<std::string, std::shared_ptr<Texture>>& textureBindingMap,
		aiMaterial* material,
		const aiTextureType& type,
		const std::string& label) const
	{
		std::vector<std::shared_ptr<Texture>> textures = LoadMaterialTextures(material, type);

		// Point to those textures by their names (label + index)
		for (int i = 0; i < textures.size(); ++i) {
			textureBindingMap[label + std::to_string(i + 1)] = textures[i];
		}
	}

	bool Model::TextureAlreadyLoaded(std::filesystem::path path) {

		// Can't do iterators and stl algorithms because references
		for (const std::shared_ptr<Texture> texture : sm_LoadedTextures) {
			if (texture->GetTexturePath() == path) {
				return true;
			}
		}

		return false;
	}

} // OORenderer
