
#include "OORenderer/Mesh.h"

#include <iostream>
#include <LoggingAD/LoggingAD.h>

#include "OORenderer/Renderer.h"
#include "OORenderer/Window.h"

namespace OORenderer {

    Mesh::Mesh(std::vector<Vertex> vertexData, std::vector<unsigned int> indices, std::map<std::string, std::shared_ptr<Texture>> textureBindingMap)
	    : m_VertexData(vertexData), m_Indices(indices), m_TextureBindingMap(textureBindingMap)
    {}

    Mesh::Mesh(const Window& window, std::vector<Vertex> vertexData, std::vector<unsigned int> indices, std::map<std::string, std::shared_ptr<Texture>> textureBindingMap)
        : Mesh(vertexData, indices, textureBindingMap)
    {
        RegisterOnWindow(window);
    }

    void Mesh::Render(ShaderProgram& shader) const {

        GLFWwindow* renderWindow = shader.GetGLFWWindow();

        auto VAOIDIt = m_WindowVAOIDMap.find(renderWindow);
        if (VAOIDIt == m_WindowVAOIDMap.end()) {
            LoggingAD::Warning("[OORenderer::Mesh::Render] Attempting to render mesh using shader registered to a window this mesh hasn't been loaded to.");
            return;
        }

        GLFWwindow* oldContext = glfwGetCurrentContext();
        Window::ActivateGLFWWindow(renderWindow);

        // Bind textures
        int i = 0;
        for (const auto& [bindingName, texture] : m_TextureBindingMap) {
            glActiveTexture(GL_TEXTURE0 + i);
            shader.SetUniform1i(bindingName.c_str(), i);
            glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
            ++i;
        }
        glActiveTexture(GL_TEXTURE0);

        // Submit the mesh to be drawn
        glBindVertexArray(VAOIDIt->second);
        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Revert context
        Window::ActivateGLFWWindow(oldContext);
    }

    void Mesh::RegisterOnGLFWWindow(GLFWwindow* window) {
        // Mesh is a OpenGL object so needs to be bound on the correct window
        GLFWwindow* oldContext = glfwGetCurrentContext();
        Window::ActivateGLFWWindow(window);

        unsigned int VAOID;

        glGenVertexArrays(1, &VAOID);

        unsigned int VBO, EBO;
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAOID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, m_VertexData.size() * sizeof(Vertex), &m_VertexData[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

        // Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

        // Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        // Tex Coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);

        m_WindowVAOIDMap[window] = VAOID;

        Window::ActivateGLFWWindow(oldContext);
    }

    void Mesh::RegisterOnWindow(const Window& window) {
        RegisterOnGLFWWindow(window.GetGLFWWindow());
    }

} // OORenderer
