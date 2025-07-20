#pragma once

#include <memory>

#include "OORenderer/ShaderProgram.h"
#include "OORenderer/Model.h"

namespace OORenderer {

	/// <summary>
	/// A renderable object, contains a model, its textures, position, and other related data
	/// </summary>
	class RenderObject {
	public: // Public methods
		RenderObject(std::shared_ptr<Model> model = nullptr, std::shared_ptr<ShaderProgram> shaderProgram = nullptr);
		RenderObject(std::filesystem::path filePath, std::shared_ptr<ShaderProgram> shaderProgram = nullptr);

		void Render() const;
		void LoadModel(std::filesystem::path filePath);
		void AssignShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram);
		void SetPVMatrix(const glm::mat4& pvMatrix, bool transpose = false);
		void RegisterOnGLFWWindow(GLFWwindow* window);
		void RegisterOnWindow(const Window& window);

		void Move(glm::vec3 vec);
		void Rotate(float angle, glm::vec3 axis = { 0.0f, 1.0f, 0.0f });
		void Scale(float scaleFactor);
		void Scale(glm::vec3 scaleFactors);

	private: // Private members
		std::shared_ptr<ShaderProgram> m_ShaderProgram;
		std::shared_ptr<Model> m_Model;

		glm::mat4 m_ModelMatrix = 1.0f; // Identity

	private: // Private static members
		inline static std::map<std::filesystem::path, std::shared_ptr<Model>> sm_LoadedModels{};
	};

} // OORenderer
