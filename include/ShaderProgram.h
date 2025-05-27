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

		/// <summary>
		/// Compile shader from source and register it with this shader program
		/// Remember to link the program after all shaders have been registered
		/// </summary>
		/// <param name="shaderSource">Shader source code. N.B. Not a path</param>
		/// <param name="shaderType">OpenGL macro for which type of shader this is</param>
		/// <returns>True if successful, false otherwise</returns>
		bool RegisterShader(const char* shaderSource, int shaderType);

		/// <summary>
		/// Load shader source from path, compile, and register that shader with this shader program
		/// Remember to link the program after all shaders have been registered
		/// </summary>
		/// <param name="shaderPath">Path to shader source file</param>
		/// <param name="shaderType">OpenGL macro for which type of shader this is</param>
		/// <returns>True if successful, false otherwise</returns>
		bool RegisterShader(std::filesystem::path shaderPath, int shaderType);

		/// <summary>
		/// Link the registered shaders together creating the complete program ready to be used
		/// </summary>
		void LinkProgram();

		/// <summary>
		/// Enable this program for use, call before submitting draw calls you wish to use this shader program
		/// </summary>
		void UseProgram();


		// These uniforms call the OpenGL uniform equivalents arrays of vectors and matrices are not currently supported
		// that is, OpenGL count values are hardcoded. E.g. SetUniformMatrix4fv assumes only 1 matrix is being set.

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

		/// <summary>
		/// Set a given uniform for this program using a given OpenGL function and appropriate arguments
		/// </summary>
		/// <typeparam name="...TArgs">Pack of types of the user arguments to be passed to the OpenGL function</typeparam>
		/// <param name="uniformName">Name of uniform to set</param>
		/// <param name="oglUniformFunction">OpenGL uniform function to use to set the value, see: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUniform.xhtml</param>
		/// <param name="...args">Arguments to pass to the OpenGL uniform function</param>
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