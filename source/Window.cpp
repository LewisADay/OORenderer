
#include "Window.h"

#include <iostream>

namespace OORenderer {

	static unsigned int s_NumWindows = 0;

	static Window* StaticGetUserOfGLFWWindow(GLFWwindow* window) {
		Window* user = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (user == NULL) {
			// TODO LOGGING
			return nullptr;
		}
	}

	static void StaticFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		Window* user = StaticGetUserOfGLFWWindow(window);
		if (!user) { return; }

		user->FramebufferSizeCallback(width, height);
	}

	static void StaticFocusCallback(GLFWwindow* window, int focused) {
		Window* user = StaticGetUserOfGLFWWindow(window);
		if (!user) { return; }

		user->FocusCallback(focused);
	}

	static void StaticKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		Window* user = StaticGetUserOfGLFWWindow(window);
		if (!user) { return; }

		user->KeyCallback(key, scancode, action, mods);
	}

	Window::Window(
		int width,
		int height,
		std::string title,
		GLFWmonitor* monitor,
		GLFWwindow* share,
		bool setToCurrent
	)
	{
		// Keep track of how many windows we have open 
		++s_NumWindows;

		// Init glfw (does nothing if alreay initialised)
		if (glfwInit() != GLFW_TRUE) {
			// TODO LOGGING
			throw "GLFW Failed to initialise aborting!";
		}

		// What sort of window do we want
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_GLFWWindow = glfwCreateWindow(width, height, title.c_str(), monitor, share);

		// Keep members up to date
		m_Width = width;
		m_Height = height;

		// Register this as the user of the glfw window for use in callbacks etc.
		glfwSetWindowUserPointer(m_GLFWWindow, this);

		// Set this to be the active window
		if (setToCurrent) {
			ActivateWindow();
		}

		glfwSetFramebufferSizeCallback(m_GLFWWindow, StaticFramebufferSizeCallback);
		glfwSetWindowFocusCallback(m_GLFWWindow, StaticFocusCallback);
		glfwSetKeyCallback(m_GLFWWindow, StaticKeyCallback);
		m_ExternFramebufferResizeCallback = nullptr;
		m_ExternFocusCallback = nullptr;
		m_ExternKeyCallback = nullptr;
	}

	Window::~Window() {
		// Keep track of how many windows we have open
		--s_NumWindows;

		// Clean up
		glfwDestroyWindow(m_GLFWWindow);

		// If no windows open shut down
		if (s_NumWindows == 0) {
			glfwTerminate();
		}
	}

	void Window::FramebufferSizeCallback(int width, int height) {
		// Keep members up to date
		m_Width = width;
		m_Height = height;

		// Size the viewport appropriately
		glViewport(0, 0, width, height);

		if (m_ExternFramebufferResizeCallback) {
			m_ExternFramebufferResizeCallback(m_GLFWWindow, width, height); // TODO Should be std::invoke? Investigate.
		}
	}

	void Window::FocusCallback(int focused) {
		if (focused) {
			std::cout << "Window gained focus: " << m_GLFWWindow << std::endl;
			ActivateWindow();
		}
		else {
			std::cout << "Window lost focus: " << m_GLFWWindow << std::endl;
		}

		if (m_ExternFocusCallback) {
			m_ExternFocusCallback(m_GLFWWindow, focused); // TODO Should be std::invoke? Investigate.
		}
	}

	GLFWframebuffersizefun Window::RegisterFramebufferResizeCallback(GLFWframebuffersizefun callback) {
		GLFWframebuffersizefun oldCallback = m_ExternFramebufferResizeCallback;
		m_ExternFramebufferResizeCallback = callback;
		return oldCallback;
	}

	GLFWkeyfun Window::RegisterKeyCallback(GLFWkeyfun callback) {
		GLFWkeyfun oldCallback = m_ExternKeyCallback;
		m_ExternKeyCallback = callback;
		return oldCallback;
	}

	GLFWwindowfocusfun Window::RegisterFocusCallback(GLFWwindowfocusfun callback)
	{
		GLFWwindowfocusfun oldCallback = m_ExternFocusCallback;
		m_ExternFocusCallback = callback;
		return oldCallback;
	}

	void Window::KeyCallback(int key, int scancode, int action, int mods) {

		if (key == GLFW_KEY_ESCAPE) {
			RequestClose();
		}

		if (m_ExternKeyCallback) {
			m_ExternKeyCallback(m_GLFWWindow, key, scancode, action, mods); // TODO Should be std::invoke? Investigate.
		}
	}

	void Window::ActivateWindow() {
		// Activate this glfw context
		glfwMakeContextCurrent(m_GLFWWindow);

		// Bind glad to this glfw context
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw "GLAD Failed to load aborting!";
		}

		// Size the viewport appropriately
		glViewport(0, 0, m_Width, m_Height);
	}

	bool Window::IsActiveWindow() {
		return m_GLFWWindow == glfwGetCurrentContext();
	}

	void Window::SetFocused() {
		glfwFocusWindow(m_GLFWWindow);
	}

	bool Window::IsFocused() {
		return glfwGetWindowAttrib(m_GLFWWindow, GLFW_FOCUSED);
	}

	GLFWwindow* Window::GetGLFWWindow() {
		return m_GLFWWindow;
	}

	bool Window::ShouldClose() {
		return glfwWindowShouldClose(m_GLFWWindow);
	}

	void Window::RequestClose() {
		glfwSetWindowShouldClose(m_GLFWWindow, true);
	}

	void Window::UpdateDisplay() {
		glfwSwapBuffers(m_GLFWWindow);
	}

	void Window::RequestAttention() {
		glfwRequestWindowAttention(m_GLFWWindow);
	}

	int Window::GetWidth() {
		return m_Width;
	}

	int Window::GetHeight() {
		return m_Height;
	}

} // OORenderer
