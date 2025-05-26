#pragma once

#include "Renderer.h"
#include <string>

namespace OORenderer {

	class Window {
	public:
		Window(
			int width,
			int height,
			std::string title = "",
			GLFWmonitor* monitor = NULL,
			GLFWwindow* share = NULL,
			bool setToCurrent = true
		);
		~Window();


		void ActivateWindow();
		bool IsActiveWindow();
		void SetFocused();
		bool IsFocused();

		bool ShouldClose();
		void RequestClose();
		void UpdateDisplay();

		void RequestAttention();
		
		int GetWidth();
		int GetHeight();

		GLFWframebuffersizefun RegisterFramebufferResizeCallback(GLFWframebuffersizefun callback);
		GLFWkeyfun RegisterKeyCallback(GLFWkeyfun callback);
		GLFWwindowfocusfun RegisterFocusCallback(GLFWwindowfocusfun callback);
		GLFWwindow* GetGLFWWindow();

	protected:
		void FramebufferSizeCallback(int width, int height);
		void FocusCallback(int focused);
		void KeyCallback(int key, int scancode, int action, int mods);

	private:
		int m_Width;
		int m_Height;
		GLFWwindow* m_GLFWWindow;
		GLFWkeyfun m_ExternKeyCallback;
		GLFWwindowfocusfun m_ExternFocusCallback;
		GLFWframebuffersizefun m_ExternFramebufferResizeCallback;

	private: // Friends
		friend static void StaticFramebufferSizeCallback(GLFWwindow* window, int width, int height);
		friend static void StaticFocusCallback(GLFWwindow* window, int focused);
		friend static void StaticKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	};

} // OORenderer
