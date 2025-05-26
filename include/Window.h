#pragma once

#include "Renderer.h"
#include <string>

namespace OORenderer {

	class Window {
	public:
		Window(int width, int height, std::string title = "", GLFWmonitor* monitor = NULL, GLFWwindow* share = NULL, bool setToCurrent = true);
		~Window();

		void FramebufferSizeCallback(int width, int height);

		void ActivateWindow();
		GLFWwindow* GetGLFWWindow();
		bool ShouldClose();
		void UpdateDisplay();

	public:
		int Width;
		int Height;

	private:
		GLFWwindow* m_GLFWWindow;
	};

} // OORenderer
