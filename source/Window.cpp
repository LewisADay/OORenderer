
#include "Window.h"

namespace OORenderer {

	static unsigned int s_NumWindows = 0;

	static void StaticFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		Window* user = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (user == NULL) {
			// TODO LOGGING
			return;
		}
		user->FramebufferSizeCallback(width, height);
	}

	Window::Window(int width, int height, std::string title, GLFWmonitor* monitor, GLFWwindow* share, bool setToCurrent) {

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

		// Set public members
		Width = width;
		Height = height;

		// Register this as the user of the glfw window for use in callbacks etc.
		glfwSetWindowUserPointer(m_GLFWWindow, this);

		// Set this to be the active window
		if (setToCurrent) {
			ActivateWindow();
		}

		

		glfwSetFramebufferSizeCallback(m_GLFWWindow, StaticFramebufferSizeCallback);
	}

	Window::~Window() {
		// Keep track of how many windows we have open
		--s_NumWindows;

		// If no windows open shut down
		if (s_NumWindows == 0) {
			glfwTerminate();
		}
	}

	void Window::FramebufferSizeCallback(int width, int height) {
		// Set public members
		Width = width;
		Height = height;

		// Size the viewport appropriately
		glViewport(0, 0, width, height);
	}

	void Window::ActivateWindow() {
		// Activate this glfw context
		glfwMakeContextCurrent(m_GLFWWindow);

		// Bind glad to this glfw context
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw "GLAD Failed to load aborting!";
		}

		// Size the viewport appropriately
		glViewport(0, 0, Width, Height);
	}

	GLFWwindow* Window::GetGLFWWindow() {
		return m_GLFWWindow;
	}

	bool Window::ShouldClose() {
		return glfwWindowShouldClose(m_GLFWWindow);
	}

	void Window::UpdateDisplay() {
		glfwSwapBuffers(m_GLFWWindow);
	}

} // OORenderer
