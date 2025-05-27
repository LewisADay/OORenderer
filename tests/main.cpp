

#include <iostream>

#include <Renderer.h>
#include <Window.h>
#include <ShaderProgram.h>

OORenderer::Window window1{ 800, 600 };
OORenderer::Window window2{ 800, 600 };

void InputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (window == window1.GetGLFWWindow()) {
		std::cout << "Window 1 in focus, keycode pressed: " << key << std::endl;
	}
	if (window == window2.GetGLFWWindow()) {
		std::cout << "Window 2 in focus, keycode pressed: " << key << std::endl;
	}
}

int main()
{
	using namespace OORenderer;
	Renderer renderer{};

	window1.RegisterKeyCallback(InputCallback);
	window2.RegisterKeyCallback(InputCallback);

	ShaderProgram shaderProgram{
		"./resources/shaders/vertShader.vs",
		"./resources/shaders/fragShader.fs"
	};

	while (!window1.ShouldClose() && !window2.ShouldClose()) {

		window1.ActivateWindow();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		window1.UpdateDisplay();

		window2.ActivateWindow();

		glClearColor(1.0, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		window2.UpdateDisplay();

		glfwPollEvents();
	}
}

