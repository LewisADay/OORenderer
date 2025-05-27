
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
		// We permit enabling a shader program for a different context than the active context
		// Ensure we're on the correct context
		GLFWwindow* oldContext = glfwGetCurrentContext();
		m_Window->ActivateWindow();

		glUseProgram(m_ProgramID);

		// Revert context
		Window::ActivateGLFWWindow(oldContext);
	}

	template<typename... TArgs>
	void ShaderProgram::SetUniformHelper(const GLchar* uniformName, void (*oglUniformFunction)(GLint, TArgs...), TArgs... args) {
		// We permit setting a uniform for a shader program in a different context than the active context
		// Ensure we're on the correct context
		GLFWwindow* oldContext = glfwGetCurrentContext();
		m_Window->ActivateWindow();

		GLint uniformLocation = glGetUniformLocation(m_ProgramID, uniformName);
		glUseProgram(m_ProgramID);
		oglUniformFunction(uniformLocation, args...);

		// Revert context
		Window::ActivateGLFWWindow(oldContext);
	}
	
	void ShaderProgram::SetUniform1f(const GLchar* uniformName, const float v0) {
		SetUniformHelper(uniformName, glUniform1f, v0);
	}

	void ShaderProgram::SetUniform2f(const GLchar* uniformName, const float v0, const float v1) {
		SetUniformHelper(uniformName, glUniform2f, v0, v1);
	}

	void ShaderProgram::SetUniform3f(const GLchar* uniformName, const float v0, const float v1, const float v2) {
		SetUniformHelper(uniformName, glUniform3f, v0, v1, v2);
	}

	void ShaderProgram::SetUniform4f(const GLchar* uniformName, const float v0, const float v1, const float v2, const float v3) {
		SetUniformHelper(uniformName, glUniform4f, v0, v1, v2, v3);
	}

	void ShaderProgram::SetUniform1i(const GLchar* uniformName, const int v0) {
		SetUniformHelper(uniformName, glUniform1i, v0);
	}

	void ShaderProgram::SetUniform2i(const GLchar* uniformName, const int v0, const int v1) {
		SetUniformHelper(uniformName, glUniform2i, v0, v1);
	}

	void ShaderProgram::SetUniform3i(const GLchar* uniformName, const int v0, const int v1, const int v2) {
		SetUniformHelper(uniformName, glUniform3i, v0, v1, v2);
	}

	void ShaderProgram::SetUniform4i(const GLchar* uniformName, const int v0, const int v1, const int v2, const int v3) {
		SetUniformHelper(uniformName, glUniform4i, v0, v1, v2, v3);
	}

	void ShaderProgram::SetUniform1ui(const GLchar* uniformName, const unsigned int v0) {
		SetUniformHelper(uniformName, glUniform1ui, v0);
	}

	void ShaderProgram::SetUniform2ui(const GLchar* uniformName, const unsigned int v0, const unsigned int v1) {
		SetUniformHelper(uniformName, glUniform2ui, v0, v1);
	}

	void ShaderProgram::SetUniform3ui(const GLchar* uniformName, const unsigned int v0, const unsigned int v1, const unsigned int v2) {
		SetUniformHelper(uniformName, glUniform3ui, v0, v1, v2);
	}

	void ShaderProgram::SetUniform4ui(const GLchar* uniformName, const unsigned int v0, const unsigned int v1, const unsigned int v2, const unsigned int v3) {
		SetUniformHelper(uniformName, glUniform4ui, v0, v1, v2, v3);
	}

	void ShaderProgram::SetUniform1fv(const GLchar* uniformName, const float* value) {
		SetUniformHelper(uniformName, glUniform1fv, 1, value);
	}

	void ShaderProgram::SetUniform2fv(const GLchar* uniformName, const float* value) {
		SetUniformHelper(uniformName, glUniform2fv, 2, value);
	}

	void ShaderProgram::SetUniform3fv(const GLchar* uniformName, const float* value) {
		SetUniformHelper(uniformName, glUniform3fv, 3, value);
	}

	void ShaderProgram::SetUniform4fv(const GLchar* uniformName, const float* value) {
		SetUniformHelper(uniformName, glUniform4fv, 4, value);
	}

	void ShaderProgram::SetUniform1iv(const GLchar* uniformName, const int* value) {
		SetUniformHelper(uniformName, glUniform1iv, 1, value);
	}

	void ShaderProgram::SetUniform2iv(const GLchar* uniformName, const int* value) {
		SetUniformHelper(uniformName, glUniform2iv, 2, value);
	}

	void ShaderProgram::SetUniform3iv(const GLchar* uniformName, const int* value) {
		SetUniformHelper(uniformName, glUniform3iv, 3, value);
	}
	
	void ShaderProgram::SetUniform4iv(const GLchar* uniformName, const int* value) {
		SetUniformHelper(uniformName, glUniform4iv, 4, value);
	}

	void ShaderProgram::SetUniform1iuv(const GLchar* uniformName, const unsigned int* value) {
		SetUniformHelper(uniformName, glUniform1uiv, 1, value);
	}

	void ShaderProgram::SetUniform2iuv(const GLchar* uniformName, const unsigned int* value) {
		SetUniformHelper(uniformName, glUniform2uiv, 2, value);
	}

	void ShaderProgram::SetUniform3iuv(const GLchar* uniformName, const unsigned int* value) {
		SetUniformHelper(uniformName, glUniform3uiv, 3, value);
	}

	void ShaderProgram::SetUniform4iuv(const GLchar* uniformName, const unsigned int* value) {
		SetUniformHelper(uniformName, glUniform4uiv, 4, value);
	}

	void ShaderProgram::SetUniformMatrix2fv(const GLchar* uniformName, const float* value, const bool transpose = false) {
		SetUniformHelper(uniformName, glUniformMatrix2fv, (GLint)1, (GLboolean)transpose, value);
	}

	void ShaderProgram::SetUniformMatrix3fv(const GLchar* uniformName, const float* value, const bool transpose = false) {
		SetUniformHelper(uniformName, glUniformMatrix3fv, (GLint)1, (GLboolean)transpose, value);
	}

	void ShaderProgram::SetUniformMatrix4fv(const GLchar* uniformName, const float* value, const bool transpose = false) {
		SetUniformHelper(uniformName, glUniformMatrix4fv, (GLint)1, (GLboolean)transpose, value);
	}

	void ShaderProgram::SetUniformMatrix2x3fv(const GLchar* uniformName, const float* value, const bool transpose = false) {
		SetUniformHelper(uniformName, glUniformMatrix2x3fv, (GLint)1, (GLboolean)transpose, value);
	}

	void ShaderProgram::SetUniformMatrix3x2fv(const GLchar* uniformName, const float* value, const bool transpose = false) {
		SetUniformHelper(uniformName, glUniformMatrix3x2fv, (GLint)1, (GLboolean)transpose, value);
	}

	void ShaderProgram::SetUniformMatrix2x4fv(const GLchar* uniformName, const float* value, const bool transpose = false) {
		SetUniformHelper(uniformName, glUniformMatrix2x4fv, (GLint)1, (GLboolean)transpose, value);
	}

	void ShaderProgram::SetUniformMatrix4x2fv(const GLchar* uniformName, const float* value, const bool transpose = false) {
		SetUniformHelper(uniformName, glUniformMatrix4x2fv, (GLint)1, (GLboolean)transpose, value);
	}

	void ShaderProgram::SetUniformMatrix3x4fv(const GLchar* uniformName, const float* value, const bool transpose = false) {
		SetUniformHelper(uniformName, glUniformMatrix3x4fv, (GLint)1, (GLboolean)transpose, value);
	}

	void ShaderProgram::SetUniformMatrix4x3fv(const GLchar* uniformName, const float* value, const bool transpose = false) {
		SetUniformHelper(uniformName, glUniformMatrix4x3fv, (GLint)1, (GLboolean)transpose, value);
	}

} // OOrenderer