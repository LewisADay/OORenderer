
#include "Camera.h"

#include <glm/glm.hpp>

namespace OORenderer {

	Camera::Camera(glm::vec3 position, glm::vec3 direction)
		: m_CameraUp(ms_WorldSpaceUp)
	{
		int windowWidth = 0;
		int windowHeight = 0;
		glfwGetFramebufferSize(glfwGetCurrentContext(), &windowWidth, &windowHeight);
		SetProjectionPerspective(1.0f, windowWidth / windowHeight, 0.1f, 100.0f);
		RecalculateMatrices();
	}

	void Camera::RecalculateMatrices() {
		// Get updated projection - should probably only do on resize callback but we do in all cases for now
		int windowWidth = 0;
		int windowHeight = 0;
		glfwGetFramebufferSize(glfwGetCurrentContext(), &windowWidth, &windowHeight);
		SetProjectionPerspective(1.0f, windowWidth / windowHeight, 0.1f, 100.0f);

		m_ViewMatrix = glm::lookAt(m_Position, m_Direction, m_CameraUp);
		m_PVMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	glm::mat4 Camera::GetProjectionMatrix() {
		return m_ProjectionMatrix;
	}

	glm::mat4 Camera::GetViewMatrix() {
		return m_ViewMatrix;
	}

	glm::mat4 Camera::GetPVMatrix() {
		return m_PVMatrix;
	}

	void Camera::LookAt(glm::vec3 position) {
		m_Direction = glm::normalize(position - m_Position);
	}

	void Camera::MoveTo(glm::vec3 position) {
		m_Position = position;
	}

	void Camera::Move(glm::vec3 displacement) {
		m_Position += displacement;
	}

	void Camera::SetProjectionOrthographic(float left, float right, float bottom, float top, float near, float far) {
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, near, far);
		m_OrthographicProjectionFlag = true;
	}

	void Camera::SetProjectionPerspective(float fovRads, float aspectRatio, float near, float far) {
		m_ProjectionMatrix = glm::perspective(fovRads, aspectRatio, near, far);
		m_OrthographicProjectionFlag = false;
	}

} // OORenderer
