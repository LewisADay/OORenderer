#pragma once

#include "OORenderer/Renderer.h"
#include <string>

namespace OORenderer {

	class Window {
	public: // Ctors and Dtors

		/// <summary>
		/// Constructor for Window class
		/// </summary>
		/// <param name="width">The desired width, in screen coordinates, of the window. This must be greater than zero.</param>
		/// <param name="height">The desired height, in screen coordinates, of the window. This must be greater than zero.</param>
		/// <param name="title">The initial, UTF-8 encoded window title.</param>
		/// <param name="monitor">The monitor to use for full screen mode, or NULL for windowed mode.</param>
		/// <param name="share">The window whose context to share resources with, or NULL to not share resources.</param>
		/// <param name="setToCurrent">Immediately activate this window or not.</param>
		Window(
			int width,
			int height,
			std::string title = "",
			GLFWmonitor* monitor = NULL,
			GLFWwindow* share = NULL,
			bool setToCurrent = true
		);
		~Window();

	public: // Public Static methods

		/// <summary>
		/// Set a given GLFWwindow to be the current GLFW context and bind OpenGL calls to it's viewport
		/// </summary>
		/// <param name="window">GLFW window to make active</param>
		static void ActivateGLFWWindow(GLFWwindow* window);

	public: // Public methods

		/// <summary>
		/// Make this window the current GLFW context and bind OpenGL calls to it's viewport
		/// </summary>
		void ActivateWindow();

		/// <summary>
		/// Determine if this window is the currently active GLFW context
		/// </summary>
		/// <returns>True if so, false otherwise</returns>
		bool IsActiveWindow() const;

		/// <summary>
		/// Set this window to be in focus (bring to foreground and take inputs)
		/// Generally try to avoid, prefer RequestAttention().
		/// </summary>
		void SetFocused();

		/// <summary>
		/// Determine if this window is currently focused
		/// </summary>
		/// <returns>True if so, false otherwise</returns>
		bool IsFocused() const;

		/// <summary>
		/// Determine if this window should close
		/// </summary>
		/// <returns>True if so, false otherwise</returns>
		bool ShouldClose() const;

		/// <summary>
		/// Request that this window close
		/// </summary>
		void RequestClose();

		/// <summary>
		/// Switch the rendering and display buffers for this window (call once per frame most likely)
		/// </summary>
		void UpdateDisplay();

		/// <summary>
		/// Request the users attention (OS specific in how this is implemented)
		/// </summary>
		void RequestAttention();
		
		/// <summary>
		/// Get this windows current width
		/// To get width in pixels please see glfwGetFramebufferSize.
		/// </summary>
		/// <returns>Current width in screen space terms</returns>
		int GetWidth() const;

		/// <summary>
		/// Get this windows current height
		/// To get height in pixels please see glfwGetFramebufferSize.
		/// </summary>
		/// <returns>Current height in screen space terms</returns>
		int GetHeight() const;

		/// <summary>
		/// Register your own callback to be called when the framebuffer resizes
		/// Only one may be registered at a time
		/// </summary>
		/// <param name="callback">Callback function to call</param>
		/// <returns>Previously set callback function</returns>
		GLFWframebuffersizefun RegisterFramebufferResizeCallback(GLFWframebuffersizefun callback);

		/// <summary>
		/// Register your own callback to be called when a key is pressed or repeated
		/// Only one may be registered at a time
		/// </summary>
		/// <param name="callback">Callback function to call</param>
		/// <returns>Previously set callback function</returns>
		GLFWkeyfun RegisterKeyCallback(GLFWkeyfun callback);

		/// <summary>
		/// Register your own callback to be called when the window gains or loses focus
		/// Only one may be registered at a time
		/// </summary>
		/// <param name="callback">Callback function to call</param>
		/// <returns>Previously set callback function</returns>
		GLFWwindowfocusfun RegisterFocusCallback(GLFWwindowfocusfun callback);

		/// <summary>
		/// Get the GLFW window this window wraps - use with caution
		/// </summary>
		/// <returns>GLFW window this window wraps</returns>
		GLFWwindow* GetGLFWWindow() const;

	protected: // Protected methods
		void FramebufferSizeCallback(int width, int height);
		void FocusCallback(int focused);
		void KeyCallback(int key, int scancode, int action, int mods);

	private: // Private members
		int m_Width;
		int m_Height;
		GLFWwindow* m_GLFWWindow;
		GLFWkeyfun m_ExternKeyCallback;
		GLFWwindowfocusfun m_ExternFocusCallback;
		GLFWframebuffersizefun m_ExternFramebufferResizeCallback;

	private: // Friends
		friend void StaticFramebufferSizeCallback(GLFWwindow* window, int width, int height);
		friend void StaticFocusCallback(GLFWwindow* window, int focused);
		friend void StaticKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	};

} // OORenderer
