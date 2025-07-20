
#include <iostream>
#include <random>
#include <ctime>

#include <LoggingAD.h>

#include "Camera.h"
#include "RenderObject.h"

class Application {
public: // Public methods
	Application()
		: m_Window1(800, 600), m_Window2(800, 600)
	{ }

	void Run() {

		Start();

		while (!m_ShouldQuit) {
			PerFrame();
		}

	}

protected:
	static void InputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		std::cout << "Window " << window << " pressed key " << key;
	}

private: // Private methods
	void Start() {

		// Enable trace level logging as an example of how to affect logging in OORenderer
		LoggingAD::LoggingConfig config = {
			.OutputLevel = LoggingAD::LogLevel::Trace
		};
		LoggingAD::SetConfig(config);

		std::filesystem::path modelPath{ "./resources/models/backpack/backpack.obj" };
		std::filesystem::path shadersPath{ "./resources/shaders/models" };
		std::string vertexShader = "vertShader.vs";
		std::string fragShader = "fragShader.fs";

		using namespace OORenderer;

		m_Window1.RegisterKeyCallback(InputCallback);
		m_Window2.RegisterKeyCallback(InputCallback);

		m_Camera.MoveTo(glm::vec3{ 0, 0, 0 });
		m_Camera.LookAt(glm::vec3{ 0, 0, -1 });

		std::shared_ptr<Model> backpackModel = std::make_shared<Model>(modelPath); // We can use the same instance of the model

		// Build our shader programs
		std::shared_ptr<ShaderProgram> shaderProgram1 = std::make_shared<ShaderProgram>(m_Window1, shadersPath / vertexShader, shadersPath / fragShader);
		std::shared_ptr<ShaderProgram> shaderProgram2 = std::make_shared<ShaderProgram>(m_Window2, shadersPath / vertexShader, shadersPath / fragShader);

		AddNewRO(m_Window1, backpackModel, shaderProgram1);
		AddNewRO(m_Window2, backpackModel, shaderProgram2);
		AddNewRO(m_Window2, backpackModel, shaderProgram2);
		AddNewRO(m_Window2, backpackModel, shaderProgram2);
		AddNewRO(m_Window2, backpackModel, shaderProgram2);
		AddNewRO(m_Window2, backpackModel, shaderProgram2);
		AddNewRO(m_Window2, backpackModel, shaderProgram2);
		AddNewRO(m_Window2, backpackModel, shaderProgram2);
		AddNewRO(m_Window2, backpackModel, shaderProgram2);
		AddNewRO(m_Window2, backpackModel, shaderProgram2);
		AddNewRO(m_Window2, backpackModel, shaderProgram2);
		AddNewRO(m_Window2, backpackModel, shaderProgram2);
		AddNewRO(m_Window2, backpackModel, shaderProgram2);
		AddNewRO(m_Window2, backpackModel, shaderProgram2);
		AddNewRO(m_Window2, backpackModel, shaderProgram2);
		AddNewRO(m_Window2, backpackModel, shaderProgram2);
}

	void PerFrame() {

		m_Window1.ActivateWindow();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_Window2.ActivateWindow();
		glEnable(GL_DEPTH_TEST);
		glClearColor(1.0, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for (auto& object : m_RenderObjects) {
			object.SetPVMatrix(m_Camera.GetPVMatrix());
			object.Render();
		}

		m_Window1.UpdateDisplay();
		m_Window2.UpdateDisplay();

		float timeValue = glfwGetTime();
		m_Camera.MoveTo(glm::vec3{ 10 * sin(timeValue), 10 * cos(timeValue), 10 });
		m_Camera.LookAt(glm::vec3{ 0, 0, 0 });
		m_Camera.RecalculateMatrices();

		glfwPollEvents();

	}

	void AddNewRO(const OORenderer::Window& window, std::shared_ptr<OORenderer::Model> model, std::shared_ptr<OORenderer::ShaderProgram> shaderProgram) {
		OORenderer::RenderObject ro{ model, shaderProgram };
		ro.RegisterOnWindow(window);
		ro.Scale(0.2f);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution dist(0.0, 5.0);

		ro.Move({ dist(gen), dist(gen), dist(gen) });

		m_RenderObjects.push_back(ro);
	}

private: // Private members
	OORenderer::Window m_Window1;
	OORenderer::Window m_Window2;

	bool m_ShouldQuit = false;
	OORenderer::Camera m_Camera;
	std::vector<OORenderer::RenderObject> m_RenderObjects;
};

int main() {
	Application app;
	app.Run();
}
