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

		// Uniforms
		void SetUniform1f(const GLchar* uniformName, const float v0);
		void SetUniform2f(const GLchar* uniformName, const float v0, const float v1);
		void SetUniform3f(const GLchar* uniformName, const float v0, const float v1, const float v2);
		void SetUniform4f(const GLchar* uniformName, const float v0, const float v1, const float v2, const float v3);

		void SetUniform1i(const GLchar* uniformName, const int v0);
		void SetUniform2i(const GLchar* uniformName, const int v0, const int v1);
		void SetUniform3i(const GLchar* uniformName, const int v0, const int v1, const int v2);
		void SetUniform4i(const GLchar* uniformName, const int v0, const int v1, const int v2, const int v3);

		void SetUniform1ui(const GLchar* uniformName, const unsigned int v0);
		void SetUniform2ui(const GLchar* uniformName, const unsigned int v0, const unsigned int v1);
		void SetUniform3ui(const GLchar* uniformName, const unsigned int v0, const unsigned int v1, const unsigned int v2);
		void SetUniform4ui(const GLchar* uniformName, const unsigned int v0, const unsigned int v1, const unsigned int v2, const unsigned int v3);

		void SetUniform1fv(const GLchar* uniformName, const float* value);
		void SetUniform2fv(const GLchar* uniformName, const float* value);
		void SetUniform3fv(const GLchar* uniformName, const float* value);
		void SetUniform4fv(const GLchar* uniformName, const float* value);

		void SetUniform1iv(const GLchar* uniformName, const int* value);
		void SetUniform2iv(const GLchar* uniformName, const int* value);
		void SetUniform3iv(const GLchar* uniformName, const int* value);
		void SetUniform4iv(const GLchar* uniformName, const int* value);

		void SetUniform1iuv(const GLchar* uniformName, const unsigned int* value);
		void SetUniform2iuv(const GLchar* uniformName, const unsigned int* value);
		void SetUniform3iuv(const GLchar* uniformName, const unsigned int* value);
		void SetUniform4iuv(const GLchar* uniformName, const unsigned int* value);

		void SetUniformMatrix2fv(const GLchar* uniformName, const float* value, const bool transpose = false);
		void SetUniformMatrix3fv(const GLchar* uniformName, const float* value, const bool transpose = false);
		void SetUniformMatrix4fv(const GLchar* uniformName, const float* value, const bool transpose = false);

		void SetUniformMatrix2x3fv(const GLchar* uniformName, const float* value, const bool transpose = false);
		void SetUniformMatrix3x2fv(const GLchar* uniformName, const float* value, const bool transpose = false);
		void SetUniformMatrix2x4fv(const GLchar* uniformName, const float* value, const bool transpose = false);
		void SetUniformMatrix4x2fv(const GLchar* uniformName, const float* value, const bool transpose = false);
		void SetUniformMatrix3x4fv(const GLchar* uniformName, const float* value, const bool transpose = false);
		void SetUniformMatrix4x3fv(const GLchar* uniformName, const float* value, const bool transpose = false);

	private: // Private methods
		template<typename... TArgs>
		void SetUniformHelper(const GLchar* uniformName, void (*oglUniformFunction)(GLint, TArgs...), TArgs... args);

	private:
		int m_ProgramID;

		// We have to register the program to a particular window
		std::shared_ptr<Window> m_Window;

		// Map from shader type e.g. GL_VERTEX_SHADER to shader ID
		std::map<int, unsigned int> m_RegisteredShaders;
	};

} // OORenderer