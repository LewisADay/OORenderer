#pragma once

#include <filesystem>
#include <map>

#include "Renderer.h"
#include "Window.h"

namespace OORenderer {

	class ShaderProgram {
	public:
		ShaderProgram(std::shared_ptr<Window> window);
		ShaderProgram(std::shared_ptr<Window> window, std::filesystem::path vertexShaderPath, std::filesystem::path fragmentShaderPath);
		~ShaderProgram();

		bool RegisterShader(const char* shaderSource, int shaderType);
		bool RegisterShader(std::filesystem::path shaderPath, int shaderType);

		void LinkProgram();

		void UseProgram();

	private:
		int m_ProgramID;

		// We have to register the program to a particular window
		std::shared_ptr<Window> m_Window;

		// Map from shader type e.g. GL_VERTEX_SHADER to shader ID
		std::map<int, unsigned int> m_RegisteredShaders;
	};

} // OORenderer