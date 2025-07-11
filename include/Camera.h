#pragma once

#include "Window.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OORenderer {

	class Camera {

	public: // Public methods
		Camera(
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 direction = glm::vec3(-1.0f, 0.0f, 0.0f)
		);

		// Call before using any matrices from this camera after any changes
		void RecalculateMatrices();
		glm::mat4 GetProjectionMatrix();
		glm::mat4 GetViewMatrix();
		glm::mat4 GetPVMatrix();

		void LookAt(glm::vec3 position);
		void MoveTo(glm::vec3 position);
		void Move(glm::vec3 displacement);

		void MoveForward(float distance);
		void MoveBackward(float distance);
		void MoveLeft(float distance);
		void MoveRight(float distance);

		void SetProjectionOrthographic(float left, float right, float bottom, float top, float near, float far);
		void SetProjectionPerspective(float fovRads, float aspectRatio, float near, float far);

	private: // Private members

		glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_Direction = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));

		const glm::vec3 ms_WorldSpaceUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_CameraUp;

		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_PVMatrix;

		// True if otherographic, false if perspective
		bool m_OrthographicProjectionFlag = false;
	};

} // OORenderer