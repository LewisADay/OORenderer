
#include "RenderObject.h"

namespace OORenderer {

	// Init static members
	std::map<std::filesystem::path, std::shared_ptr<Model>> RenderObject::sm_LoadedModels{};

	RenderObject::RenderObject(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> shaderProgram)
		: m_Model(model), m_ShaderProgram(shaderProgram)
	{}

	RenderObject::RenderObject(std::filesystem::path filePath, std::shared_ptr<ShaderProgram> shaderProgram)
		: m_ShaderProgram(shaderProgram)
	{
		LoadModel(filePath);
	}

	void RenderObject::Render() const {
		m_ShaderProgram->SetUniformMatrix4fv("modelMatrix", m_ModelMatrix);
		m_Model->Render(*m_ShaderProgram);
	}

	void RenderObject::LoadModel(std::filesystem::path filePath) {
		auto alreadyLoadedIt = sm_LoadedModels.find(filePath);
		if (alreadyLoadedIt == sm_LoadedModels.end()) {
			m_Model = std::make_shared<Model>(filePath);
			sm_LoadedModels[filePath] = m_Model;
		}
		else {
			m_Model = sm_LoadedModels[filePath];
		}
	}

	void RenderObject::AssignShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram) {
		m_ShaderProgram = shaderProgram;
	}

	void RenderObject::SetPVMatrix(const glm::mat4& pvMatrix, bool transpose) {
		m_ShaderProgram->SetUniformMatrix4fv("pvMatrix", pvMatrix, transpose);
	}

	void RenderObject::RegisterOnGLFWWindow(GLFWwindow* window) {
		m_Model->RegisterOnGLFWWindow(window);
	}

	void RenderObject::RegisterOnWindow(const Window& window) {
		m_Model->RegisterOnWindow(window);
	}

	void RenderObject::Move(glm::vec3 vec) {
		m_ModelMatrix = glm::translate(m_ModelMatrix, vec);
	}

	void RenderObject::Rotate(float angle, glm::vec3 axis) {
		m_ModelMatrix = glm::rotate(m_ModelMatrix, angle, axis);
	}

	void RenderObject::Scale(float scaleFactor) {
		Scale(glm::vec3{ scaleFactor });
	}

	void RenderObject::Scale(glm::vec3 scaleFactors) {
		m_ModelMatrix = glm::scale(m_ModelMatrix, scaleFactors);
	}

} // OORenderer
