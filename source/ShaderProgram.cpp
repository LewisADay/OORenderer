
#include "ShaderProgram.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>

namespace OORenderer {

	ShaderProgram::ShaderProgram(std::shared_ptr<Window> window)
		: m_Window(window)
	{
		GLFWwindow* oldContext = glfwGetCurrentContext();
		window->ActivateWindow();
		m_ProgramID = glCreateProgram();
		Window::ActivateGLFWWindow(oldContext);
	}

	ShaderProgram::ShaderProgram(std::shared_ptr<Window> window, std::filesystem::path vertexShaderPath, std::filesystem::path fragmentShaderPath)
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
		m_Window->ActivateWindow();

		glDeleteProgram(m_ProgramID);

		Window::ActivateGLFWWindow(oldContext);
	}

	bool ShaderProgram::RegisterShader(const char* shaderSource, int shaderType) {

		// Ensure we're on the correct context
		GLFWwindow* oldContext = glfwGetCurrentContext();
		m_Window->ActivateWindow();

		unsigned int shaderID = glCreateShader(shaderType);
		glShaderSource(shaderID, 1, &shaderSource, NULL);
		glCompileShader(shaderID);

		int  success;
		char infoLog[512];
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog << std::endl; // TODO LOGGING
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
			std::cerr << "Could not find shader at path: " + shaderPath.string() + "." << std::endl; // TODO LOGGING
			return false;
		}
		std::string fileContents(std::filesystem::file_size(shaderPath), '\0');
		shaderFile.read(fileContents.data(), fileContents.size());

		return RegisterShader(fileContents.c_str(), shaderType);
	}

	void ShaderProgram::LinkProgram() {

		// Ensure we're on the correct context
		GLFWwindow* oldContext = glfwGetCurrentContext();
		m_Window->ActivateWindow();

		for (auto& [type, id] : m_RegisteredShaders) {
			glAttachShader(m_ProgramID, id);
			std::cout << "Attaching shader of type: " << type << " with ID: " << id << std::endl; // TODO LOGGING
		}

		glLinkProgram(m_ProgramID);

		int  success;
		char infoLog[512];
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_ProgramID, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADERPROGRAM::" << m_ProgramID << "::LINKING_FAILED\n" << infoLog << std::endl; // TODO LOGGING
		}

		// Shaders have been used no need to hold onto them
		for (auto& [type, id] : m_RegisteredShaders) {
			glDeleteShader(id);
		}

		// Revert context
		Window::ActivateGLFWWindow(oldContext);
	}

	void ShaderProgram::UseProgram() {
		// We permit enabling a shaderprogram for a different context than the active context
		// Ensure we're on the correct context
		GLFWwindow* oldContext = glfwGetCurrentContext();
		m_Window->ActivateWindow();

		glUseProgram(m_ProgramID);

		// Revert context
		Window::ActivateGLFWWindow(oldContext);
	}

} // OOrenderer