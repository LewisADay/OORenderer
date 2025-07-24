#pragma once

#include "OORenderer/Window.h"

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

		/// <summary>
		/// Recalculate the view and PV matrices.
		/// Do this between changes to the camera e.g. after calling Move()
		/// and using the camera matrices e.g. passing to a shaderprogram.
		/// </summary>
		void RecalculateMatrices();

		/// <summary>
		/// Retrieve this cameras projection matrix
		/// </summary>
		/// <returns>Projection matrix</returns>
		glm::mat4 GetProjectionMatrix() const;

		/// <summary>
		/// Retrieve this cameras view matrix
		/// </summary>
		/// <returns>View matrix</returns>
		glm::mat4 GetViewMatrix() const;

		/// <summary>
		/// Retrieve this cameras PV matrix (Projection * View)
		/// </summary>
		/// <returns>PV matrix</returns>
		glm::mat4 GetPVMatrix() const;

		/// <summary>
		/// Turn camera to look at a point in world space
		/// </summary>
		/// <param name="position">Position to look at</param>
		void LookAt(glm::vec3 position);
		
		/// <summary>
		/// Move camera to point in world space
		/// </summary>
		/// <param name="position">Position to move to</param>
		void MoveTo(glm::vec3 position);

		/// <summary>
		/// Move the camera
		/// </summary>
		/// <param name="displacement">Amount and direction to move the camera</param>
		void Move(glm::vec3 displacement);

		/// <summary>
		/// Move the camera in the direction it is facing by some distance
		/// </summary>
		/// <param name="distance">Distance to move the camera</param>
		void MoveForward(float distance);

		/// <summary>
		/// Move the camera in the opposite direction to the one it is facing by some distance
		/// </summary>
		/// <param name="distance">Distance to move the camera</param>
		void MoveBackward(float distance);

		/// <summary>
		/// Move the camera in the direction to it's left by some distance
		/// </summary>
		/// <param name="distance">Distance to move the camera</param>
		void MoveLeft(float distance);

		/// <summary>
		/// Move the camera in the direction to it's right by some distance
		/// </summary>
		/// <param name="distance">Distance to move the camera</param>
		void MoveRight(float distance);

		/// <summary>
		/// Set the cameras projection matrix to orthographic with standard plane inputs
		/// </summary>
		void SetProjectionOrthographic(float left, float right, float bottom, float top, float near, float far);

		/// <summary>
		/// Set the cameras projection matrix to perspective with standard glm perspective inputs
		/// </summary>
		/// <param name="fovRads">Field of view in radians</param>
		/// <param name="aspectRatio">Aspect ratio of display</param>
		/// <param name="near">Distance to near plane</param>
		/// <param name="far">Distance to far plane</param>
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