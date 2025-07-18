

#include <iostream>

#include <Renderer.h>
#include <Window.h>
#include <ShaderProgram.h>
#include <Camera.h>
#include <Model.h>

void InputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	std::cout << "Window " << window << " pressed key " << key;
}

int main()
{
	std::cout << "Vertex Shader Enum: " << GL_VERTEX_SHADER << std::endl;
	std::cout << "Frag Shader Enum: " << GL_FRAGMENT_SHADER << std::endl;

	std::filesystem::path modelPath{ "./resources/models/backpack/backpack.obj" };
	std::filesystem::path shadersPath{ "./resources/shaders/models" };
	std::string vertexShader = "vertShader.vs";
	std::string fragShader = "fragShader.fs";

	using namespace OORenderer;

	Window window1{ 800, 600 };
	Window window2{ 800, 600 };

	window1.RegisterKeyCallback(InputCallback);
	window2.RegisterKeyCallback(InputCallback);

	// Advanced construction arbitrary shader stages as supported by OpenGL
	ShaderProgram shaderProgram1{ window1 };

	shaderProgram1.RegisterShader(shadersPath / vertexShader, GL_VERTEX_SHADER);
	shaderProgram1.RegisterShader(shadersPath / fragShader, GL_FRAGMENT_SHADER);

	shaderProgram1.LinkProgram();


	// Simple construction
	ShaderProgram shaderProgram2{ window2, shadersPath / vertexShader, shadersPath / fragShader };


	// Setup camera - we use the same camera for both windows because that's an option
	Camera camera1;

	std::cout << "Begin loading model...\n";
	Model backpackModel{ modelPath };
	std::cout << "End loading model.\n";

	// Register the model for use on both windows
	backpackModel.RegisterOnWindow(window1);
	backpackModel.RegisterOnWindow(window2);

	glm::mat4 modelMatrix{ 1.0f }; // Identity

	while (!window1.ShouldClose() && !window2.ShouldClose()) {

		window1.ActivateWindow();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram1.SetUniformMatrix4fv("pvMatrix", camera1.GetPVMatrix());
		shaderProgram1.SetUniformMatrix4fv("modelMatrix", modelMatrix);
		shaderProgram1.UseProgram();

		backpackModel.Render(shaderProgram1);

		window1.UpdateDisplay();



		window2.ActivateWindow();
		glEnable(GL_DEPTH_TEST);
		glClearColor(1.0, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram2.SetUniformMatrix4fv("pvMatrix", camera1.GetPVMatrix());
		shaderProgram2.SetUniformMatrix4fv("modelMatrix", modelMatrix);
		shaderProgram2.UseProgram();

		backpackModel.Render(shaderProgram2);

		window2.UpdateDisplay();

		float timeValue = glfwGetTime();
		camera1.MoveTo(glm::vec3{ 10*sin(timeValue), 10*cos(timeValue), 10 });
		camera1.LookAt(glm::vec3{ 0, 0, 0 });
		camera1.RecalculateMatrices();

		glfwPollEvents();
	}
}

