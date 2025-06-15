
#include "Mesh.h"

#include "Renderer.h"
#include "Window.h"

namespace OORenderer {

    Mesh::Mesh(const Window& window, std::vector<Vertex> vertexData, std::vector<unsigned int> indices, std::map<std::string, unsigned int> textureBindingMap)
	    : m_Window(window.GetGLFWWindow()), m_VertexData(vertexData), m_Indices(indices), m_TextureBindingMap(textureBindingMap)
    {
        // Mesh is a OpenGL object so needs to be bound on the correct window
        GLFWwindow* oldContext = glfwGetCurrentContext();
        Window::ActivateGLFWWindow(m_Window);

        glGenVertexArrays(1, &m_VAOID);

        unsigned int VBO, EBO;
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(m_VAOID);
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

        Window::ActivateGLFWWindow(oldContext);
    }

    void Mesh::Render(ShaderProgram& shader) {

        // Bind textures
        int i = 0;
        for (const auto& [bindingName, textureID] : m_TextureBindingMap) {
            glActiveTexture(GL_TEXTURE0 + i);
            shader.SetUniform1i(bindingName.c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textureID);
            ++i;
        }
        glActiveTexture(GL_TEXTURE0);

        // Submit the mesh to be drawn
        glBindVertexArray(m_VAOID);
        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

} // OORenderer
