#pragma once

#include <vector>
#include <map>
#include <string>

#include <glm/glm.hpp>

#include "ShaderProgram.h"
#include "Texture.h"

namespace OORenderer {

	class Mesh {

	public: // Public objects

		struct Vertex {
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
		};

	public: // Public methods

		Mesh(std::vector<Vertex> vertexData, std::vector<unsigned int> indices, std::map<std::string, std::shared_ptr<Texture>> textureBindingMap);
		Mesh(const Window& window, std::vector<Vertex> vertexData, std::vector<unsigned int> indices, std::map<std::string, std::shared_ptr<Texture>> textureBindingMap);
		void Render(ShaderProgram& shader) const;
		void RegisterOnGLFWWindow(GLFWwindow* window);
		void RegisterOnWindow(const Window& window);

	private:
		// For each window this mesh is registered on, return the VAO ID associated with this mesh
		std::map<GLFWwindow*, unsigned int> m_WindowVAOIDMap{};

		std::vector<Vertex> m_VertexData;
		std::vector<unsigned int> m_Indices;
		std::map<std::string, std::shared_ptr<Texture>> m_TextureBindingMap;
	};

} // OORenderer