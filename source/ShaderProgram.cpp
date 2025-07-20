
#include "OORenderer/ShaderProgram.h"
#include "OORenderer/Renderer.h"

#include <iostream>
#include <fstream>
#include <LoggingAD/LoggingAD.h>

namespace OORenderer {

	ShaderProgram::ShaderProgram(const Window& window)
		: m_Window(window.GetGLFWWindow())
	{
		GLFWwindow* oldContext = glfwGetCurrentContext();
		Window::ActivateGLFWWindow(m_Window);
		m_ProgramID = glCreateProgram();
		Window::ActivateGLFWWindow(oldContext);
	}

	ShaderProgram::ShaderProgram(const Window& window, std::filesystem::path vertexShaderPath, std::filesystem::path fragmentShaderPath)
		: ShaderProgram(window)
	{
		RegisterShader(vertexShaderPath, GL_VERTEX_SHADER);
		RegisterShader(fragmentShaderPath, GL_FRAGMENT_SHADER);
		LinkProgram();
	}

	ShaderProgram::~ShaderProgram() {
		// Uncertain if this is necessary - seems to work without but this shouldn't be called too often
		// (unless you're doing something odd in which case on your own head be it)
		// Ensure we're on the correct context
		GLFWwindow* oldContext = glfwGetCurrentContext();
		Window::ActivateGLFWWindow(m_Window);

		glDeleteProgram(m_ProgramID);

		Window::ActivateGLFWWindow(oldContext);
	}

	bool ShaderProgram::RegisterShader(const char* shaderSource, int shaderType) {

		// Ensure we're on the correct context
		GLFWwindow* oldContext = glfwGetCurrentContext();
		Window::ActivateGLFWWindow(m_Window);

		unsigned int shaderID = glCreateShader(shaderType);
		glShaderSource(shaderID, 1, &shaderSource, NULL);
		glCompileShader(shaderID);

		int  success;
		char infoLog[512];
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
			LoggingAD::Error("[OORenderer::ShaderProgram::Compilation] Compilation failed of shader of type {}. With log: {}", shaderType, std::string(infoLog));
			LoggingAD::Info("[OORenderer::ShaderProgram::Compilation] Compilation failed of shader with source: {}", std::string(shaderSource));
			// Revert context
			Window::ActivateGLFWWindow(oldContext);
			return false;
		}

		glAttachShader(m_ProgramID, shaderID);
		m_RegisteredShaders[shaderType] = shaderID;

		// Revert context
		Window::ActivateGLFWWindow(oldContext);
		return true;
	}

	bool ShaderProgram::RegisterShader(std::filesystem::path shaderPath, int shaderType) {
		// Get shader source
		std::ifstream shaderFile(shaderPath, std::ios::in);
		if (!shaderFile.is_open()) {
			LoggingAD::Error("[OORenderer::ShaderProgram::Load] Failed to load shader at path: {}", shaderPath.string());
			return false;
		}
		std::string fileContents(std::filesystem::file_size(shaderPath), '\0');
		shaderFile.read(fileContents.data(), fileContents.size());

		return RegisterShader(fileContents.c_str(), shaderType);
	}

	void ShaderProgram::LinkProgram() {

		// Ensure we're on the correct context
		GLFWwindow* oldContext = glfwGetCurrentContext();
		Window::ActivateGLFWWindow(m_Window);

		for (auto [type, id] : m_RegisteredShaders) {
			glAttachShader(m_ProgramID, id);
			LoggingAD::Trace("[OORenderer::ShaderProgram::Linking] Attaching shader of type: {} with ID: {} to window: {:#010x}", type, id, reinterpret_cast<std::uintptr_t>(m_Window));
		}

		glLinkProgram(m_ProgramID);

		int  success;
		char infoLog[512];
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_ProgramID, 512, NULL, infoLog);
			LoggingAD::Error("[OORenderer::ShaderProgram::Linking] Linking shader program with id {} to window {:#010x} failed. With log: {}", m_ProgramID, reinterpret_cast<std::uintptr_t>(m_Window), std::string(infoLog));
		}

		// Shaders have been used no need to hold onto them
		for (auto& [type, id] : m_RegisteredShaders) {
			glDeleteShader(id);
		}

		// Revert context
		Window::ActivateGLFWWindow(oldContext);
	}

	void ShaderProgram::UseProgram() {
		// We permit enabling a shader program for a different context than the active context
		// Ensure we're on the correct context
		GLFWwindow* oldContext = glfwGetCurrentContext();
		Window::ActivateGLFWWindow(m_Window);

		glUseProgram(m_ProgramID);

		// Revert context
		Window::ActivateGLFWWindow(oldContext);
	}

	GLFWwindow* ShaderProgram::GetGLFWWindow() const {
		return m_Window;
	}

} // OOrenderer