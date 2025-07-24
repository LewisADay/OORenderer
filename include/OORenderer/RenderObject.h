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

		/// <summary>
		/// Render this object
		/// </summary>
		void Render() const;

		/// <summary>
		/// Load the model at the given path
		/// </summary>
		/// <param name="filePath">Path to model file</param>
		void LoadModel(std::filesystem::path filePath);

		/// <summary>
		/// Assign a shader program to this render object
		/// </summary>
		/// <param name="shaderProgram"></param>
		void AssignShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram);

		/// <summary>
		/// Provide a PV (Perspective * View) matrix to pass to this models shader
		/// </summary>
		/// <param name="pvMatrix">Matrix to set</param>
		/// <param name="transpose">Does it need to be transposed?</param>
		void SetPVMatrix(const glm::mat4& pvMatrix, bool transpose = false);

		/// <summary>
		/// Register this renderobject to render of a given GLFWwindow context
		/// </summary>
		/// <param name="window">Window to register to</param>
		void RegisterOnGLFWWindow(GLFWwindow* window);

		/// <summary>
		/// Register this renderobject to render of a given Window context
		/// </summary>
		/// <param name="window">Window to register to</param>
		void RegisterOnWindow(const Window& window);

		/// <summary>
		/// Move this object
		/// </summary>
		/// <param name="vec">Amount and direction to move the object</param>
		void Move(glm::vec3 vec);

		/// <summary>
		/// Rotate this object
		/// </summary>
		/// <param name="angle">Angle (radians) to rotate</param>
		/// <param name="axis">Axis of rotation</param>
		void Rotate(float angle, glm::vec3 axis = { 0.0f, 1.0f, 0.0f });

		/// <summary>
		/// Scale the object
		/// </summary>
		/// <param name="scaleFactor">Scale factor to scale the object by</param>
		void Scale(float scaleFactor);

		/// <summary>
		/// Scale the object
		/// </summary>
		/// <param name="scaleFactors">Scale factor(s) to scale the object by in each dimension</param>
		void Scale(glm::vec3 scaleFactors);

	private: // Private members
		std::shared_ptr<ShaderProgram> m_ShaderProgram;
		std::shared_ptr<Model> m_Model;

		glm::mat4 m_ModelMatrix = 1.0f; // Identity

	private: // Private static members
		inline static std::map<std::filesystem::path, std::shared_ptr<Model>> sm_LoadedModels{};
	};

} // OORenderer
