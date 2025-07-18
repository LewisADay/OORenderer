
#include "ShaderProgram.h"

namespace OORenderer {

	template<typename... TArgs>
	void ShaderProgram::SetUniformHelper(const GLchar* uniformName, void (*oglUniformFunction)(GLint, TArgs...), TArgs... args) {
		// We permit setting a uniform for a shader program in a different context than the active context
		// Ensure we're on the correct context
		GLFWwindow* oldContext = glfwGetCurrentContext();
		Window::ActivateGLFWWindow(m_Window);

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

	void ShaderProgram::SetUniformMatrix2fv(const GLchar* uniformName, const float* value, const bool transpose) {
		SetUniformHelper(uniformName, glUniformMatrix2fv, (GLint)1, (GLboolean)transpose, value);
	}

	void ShaderProgram::SetUniformMatrix3fv(const GLchar* uniformName, const float* value, const bool transpose) {
		SetUniformHelper(uniformName, glUniformMatrix3fv, (GLint)1, (GLboolean)transpose, value);
	}

	void ShaderProgram::SetUniformMatrix4fv(const GLchar* uniformName, const float* value, const bool transpose) {
		SetUniformHelper(uniformName, glUniformMatrix4fv, (GLint)1, (GLboolean)transpose, value);
	}

	void ShaderProgram::SetUniformMatrix2x3fv(const GLchar* uniformName, const float* value, const bool transpose) {
		SetUniformHelper(uniformName, glUniformMatrix2x3fv, (GLint)1, (GLboolean)transpose, value);
	}

	void ShaderProgram::SetUniformMatrix3x2fv(const GLchar* uniformName, const float* value, const bool transpose) {
		SetUniformHelper(uniformName, glUniformMatrix3x2fv, (GLint)1, (GLboolean)transpose, value);
	}

	void ShaderProgram::SetUniformMatrix2x4fv(const GLchar* uniformName, const float* value, const bool transpose) {
		SetUniformHelper(uniformName, glUniformMatrix2x4fv, (GLint)1, (GLboolean)transpose, value);
	}

	void ShaderProgram::SetUniformMatrix4x2fv(const GLchar* uniformName, const float* value, const bool transpose) {
		SetUniformHelper(uniformName, glUniformMatrix4x2fv, (GLint)1, (GLboolean)transpose, value);
	}

	void ShaderProgram::SetUniformMatrix3x4fv(const GLchar* uniformName, const float* value, const bool transpose) {
		SetUniformHelper(uniformName, glUniformMatrix3x4fv, (GLint)1, (GLboolean)transpose, value);
	}

	void ShaderProgram::SetUniformMatrix4x3fv(const GLchar* uniformName, const float* value, const bool transpose) {
		SetUniformHelper(uniformName, glUniformMatrix4x3fv, (GLint)1, (GLboolean)transpose, value);
	}

	void ShaderProgram::SetUniform2fv(const GLchar* uniformName, const glm::vec2& value) {
		SetUniform2fv(uniformName, &value[0]);
	}

	void ShaderProgram::SetUniform3fv(const GLchar* uniformName, const glm::vec3& value) {
		SetUniform3fv(uniformName, &value[0]);
	}

	void ShaderProgram::SetUniform4fv(const GLchar* uniformName, const glm::vec4& value) {
		SetUniform4fv(uniformName, &value[0]);
	}

	void ShaderProgram::SetUniformMatrix2fv(const GLchar* uniformName, const glm::mat2& value, const bool transpose) {
		SetUniformMatrix2fv(uniformName, &value[0][0], transpose);
	}

	void ShaderProgram::SetUniformMatrix3fv(const GLchar* uniformName, const glm::mat3& value, const bool transpose) {
		SetUniformMatrix3fv(uniformName, &value[0][0], transpose);
	}

	void ShaderProgram::SetUniformMatrix4fv(const GLchar* uniformName, const glm::mat4& value, const bool transpose) {
		SetUniformMatrix4fv(uniformName, &value[0][0], transpose);
	}

} // OORenderer
