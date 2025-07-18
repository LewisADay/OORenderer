#pragma once

#include <filesystem>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ShaderProgram.h"
#include "Mesh.h"
#include "Texture.h"

namespace OORenderer {

	class Model {
	public: // Public Methods
		Model(std::filesystem::path path);

		/// <summary>
		/// Render this model using the provided shader
		/// </summary>
		/// <param name="shader">Shader program to render this model with</param>
		void Render(ShaderProgram& shader);

		/// <summary>
		/// Register this model for renderering on a given window - this allows us to only load a model once for use on multiple windows
		/// </summary>
		/// <param name="window">GLFW window ptr of window to register to</param>
		void RegisterOnGLFWWindow(GLFWwindow* window);

		/// <summary>
		/// Register this model for renderering on a given window - this allows us to only load a model once for use on multiple windows
		/// </summary>
		/// <param name="window">Window to register to</param>
		void RegisterOnWindow(const Window& window);

	private: // Private Methods
		void LoadFromPath(std::filesystem::path path);
		void ProcessASSIMPNode(aiNode* node, const aiScene* scene);
		void ProcessASSIMPMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* material, aiTextureType type) const;
		void BuildTextureBindingMapFromMaterial(std::map<std::string, std::shared_ptr<Texture>>& textureBindingMap, aiMaterial* material, const aiTextureType& type, const std::string& label) const;

	private: // Private Static Methods
		static bool TextureAlreadyLoaded(std::filesystem::path path);

	private: // Private Static Members
		static std::vector<std::shared_ptr<Texture>> sm_LoadedTextures; // Prevent duplicate loading of textures

	private: // Private Members
		std::vector<Mesh> m_Meshes;
		std::filesystem::path m_ModelDirectory;
		std::vector<GLFWwindow*> m_RegisteredWindows;

		std::vector<std::shared_ptr<Texture>> m_DiffuseMaps;
		std::vector<std::shared_ptr<Texture>> m_SpecularMaps;
		std::vector<std::shared_ptr<Texture>> m_AmbientMaps;
		std::vector<std::shared_ptr<Texture>> m_NormalMaps;
		std::vector<std::shared_ptr<Texture>> m_HeightMaps;

	};

} // OORenderer
