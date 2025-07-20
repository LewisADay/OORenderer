
#include "Window.h"

#include <iostream>
#include <LoggingAD.h>

namespace OORenderer {

	static unsigned int s_NumWindows = 0;

	static Window* StaticGetUserOfGLFWWindow(GLFWwindow* window) {
		Window* user = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (user == nullptr) {
			LoggingAD::Error("[OORenderer::Window::Utils] Attempting to fetch user of GLFW window and none found!");
			return nullptr;
		}
		return user;
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
		LoggingAD::Trace("Creating window with title: {}.", title);

		// Keep track of how many windows we have open 
		++s_NumWindows;

		// Init glfw (does nothing if alreay initialised)
		if (glfwInit() != GLFW_TRUE) {
			LoggingAD::Error("[OORenderer::Window::Init] GLFW Failed to initialise! Aborting.");
			throw "[OORenderer::Window::Init] GLFW Failed to initialise aborting!";
		}

		// What sort of window do we want
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_GLFWWindow = glfwCreateWindow(width, height, title.c_str(), monitor, share);

		LoggingAD::Trace("Creating GLFW context for window with title: {}. GLFW Window: {:#010x}", title, reinterpret_cast<std::uintptr_t>(m_GLFWWindow));

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

		LoggingAD::Trace("Destroying window {:#010x}", reinterpret_cast<std::uintptr_t>(m_GLFWWindow));

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

		LoggingAD::Trace("Resizing window {:#010x}, to size ({}, {})", reinterpret_cast<std::uintptr_t>(m_GLFWWindow), width, height);

		// Keep members up to date
		m_Width = width;
		m_Height = height;

		// Size the viewport appropriately
		int widthPx, heightPx;
		glfwGetFramebufferSize(m_GLFWWindow, &widthPx, &heightPx);
		glViewport(0, 0, width, height);

		if (m_ExternFramebufferResizeCallback) {
			m_ExternFramebufferResizeCallback(m_GLFWWindow, width, height); // TODO Should be std::invoke? Investigate.
		}
	}

	void Window::FocusCallback(int focused) {
		if (focused) {
			LoggingAD::Trace("[OORenderer::Window] Window gained focus: {:#010x}", reinterpret_cast<std::uintptr_t>(m_GLFWWindow));
			ActivateWindow();
		}
		else {
			LoggingAD::Trace("[OORenderer::Window] Window lost focus: {:#010x}", reinterpret_cast<std::uintptr_t>(m_GLFWWindow));
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

	void Window::ActivateGLFWWindow(GLFWwindow* window) {
		// Already active?
		if (window == glfwGetCurrentContext()) {
			return;
		}

		// Activate this glfw context
		glfwMakeContextCurrent(window);

		// Bind glad to this glfw context
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw "GLAD Failed to load aborting!";
		}

		// Size the viewport appropriately
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}
	
	void Window::ActivateWindow() {
		ActivateGLFWWindow(m_GLFWWindow);
	}

	bool Window::IsActiveWindow() const {
		return m_GLFWWindow == glfwGetCurrentContext();
	}

	void Window::SetFocused() {
		glfwFocusWindow(m_GLFWWindow);
	}

	bool Window::IsFocused() const {
		return glfwGetWindowAttrib(m_GLFWWindow, GLFW_FOCUSED);
	}

	GLFWwindow* Window::GetGLFWWindow() const {
		return m_GLFWWindow;
	}

	bool Window::ShouldClose() const {
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

	int Window::GetWidth() const {
		return m_Width;
	}

	int Window::GetHeight() const {
		return m_Height;
	}

} // OORenderer
