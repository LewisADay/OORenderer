#pragma once

#include <filesystem>
#include <map>

namespace OORenderer {

	class ShaderProgram {
	public:
		ShaderProgram();
		ShaderProgram(std::filesystem::path vertexShaderPath, std::filesystem::path fragmentShaderPath);
		~ShaderProgram();

		bool RegisterShader(const char* shaderSource, int shaderType);
		bool RegisterShader(std::filesystem::path shaderPath, int shaderType);

		void LinkProgram();

		void UseProgram();

	private:
		int m_ProgramID;

		// Map from shader type e.g. GL_VERTEX_SHADER to shader ID
		std::map<unsigned int, int> m_RegisteredShaders;
	};

} // OORenderer