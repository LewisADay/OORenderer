#pragma once

#include <vector>
#include <map>
#include <string>

#include <glm/glm.hpp>

#include "ShaderProgram.h"

namespace OORenderer {

	class Mesh {

	public: // Public objects

		struct Vertex {
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
		};

	public: // Public methods

		Mesh(const Window& window, std::vector<Vertex> vertexData, std::vector<unsigned int> indices, std::map<std::string, unsigned int> textureBindingMap);
		void Render(ShaderProgram& shader);

	private:
		// We have to register the mesh to a particular opengl viewport
		GLFWwindow* m_Window;

		std::vector<Vertex> m_VertexData;
		std::vector<unsigned int> m_Indices;
		std::map<std::string, unsigned int> m_TextureBindingMap;

		unsigned int m_VAOID;
	};

} // OORenderer