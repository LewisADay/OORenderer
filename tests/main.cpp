

#include <iostream>

#include <Renderer.h>
#include <Window.h>

int main()
{
	using namespace OORenderer;
	Renderer renderer{};
	Window window{ 800, 600 };
	Window window2{ 800, 600 };

	while (!window.ShouldClose() && !window2.ShouldClose()) {
		window.UpdateDisplay();
		window2.UpdateDisplay();
		glfwPollEvents();
	}
}

