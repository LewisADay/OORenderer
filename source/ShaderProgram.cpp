
#include "ShaderProgram.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>

namespace OORenderer {

	ShaderProgram::ShaderProgram() {
		m_ProgramID = glCreateProgram();
	}

	ShaderProgram::ShaderProgram(std::filesystem::path vertexShaderPath, std::filesystem::path fragmentShaderPath)
		: ShaderProgram()
	{
		RegisterShader(vertexShaderPath, GL_VERTEX_SHADER);
		RegisterShader(fragmentShaderPath, GL_FRAGMENT_SHADER);
	}

	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(m_ProgramID);
	}

	bool ShaderProgram::RegisterShader(const char* shaderSource, int shaderType) {
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
			return false;
		}

		glAttachShader(m_ProgramID, shaderID);
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

		RegisterShader(fileContents.c_str(), shaderType);
		return true;
	}

	void ShaderProgram::LinkProgram() {
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
	}

	void ShaderProgram::UseProgram() {
		glUseProgram(m_ProgramID);
	}

} // OOrenderer