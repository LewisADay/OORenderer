

#include <iostream>

#include <glad/glad.h>

#include <OORenderer/Window.h>
#include <OORenderer/ShaderProgram.h>
#include <OORenderer/Texture.h>
#include <OORenderer/Camera.h>


void InputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	std::cout << "Window " << window << " pressed key " << key;
}

/// <summary>
/// Static buffer setup for test
/// </summary>
/// <returns>ID of VAO we've setup</returns>
int SetupBuffers() {
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	return VAO;
}

int main()
{
	std::cout << "Vertex Shader Enum: " << GL_VERTEX_SHADER << std::endl;
	std::cout << "Frag Shader Enum: " << GL_FRAGMENT_SHADER << std::endl;

	std::filesystem::path texturePath1{ "./resources/textures/container.jpg" };
	std::filesystem::path texturePath2{ "./resources/textures/wall.jpg" };
	std::filesystem::path shadersPath{ "./resources/shaders/Cameras" };
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

	// We need to be on the correct context when we build the buffers
	window1.ActivateWindow();
	unsigned int VAO1 = SetupBuffers();
	window2.ActivateWindow();
	unsigned int VAO2 = SetupBuffers();

	// Setup textures
	Texture texture1{ window1, texturePath1 };
	Texture texture2{ window2, texturePath2 };

	// Setup camera - we use the same camera for both windows because that's an option
	Camera camera1;

	glm::mat4 modelMatrix{ 1.0f }; // Identity
	std::cout << modelMatrix[0][0] << ' ' << modelMatrix[0][1] << ' ' << modelMatrix[0][2] << ' ' << modelMatrix[0][3] << std::endl;

	while (!window1.ShouldClose() && !window2.ShouldClose()) {

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

		window1.ActivateWindow();

		glBindTexture(GL_TEXTURE_2D, texture1.GetTextureID());
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram1.SetUniformMatrix4fv("pvMatrix", camera1.GetPVMatrix());
		shaderProgram1.SetUniformMatrix4fv("modelMatrix", modelMatrix);
		shaderProgram1.UseProgram();

		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		window1.UpdateDisplay();



		window2.ActivateWindow();

		glBindTexture(GL_TEXTURE_2D, texture2.GetTextureID());
		glClearColor(1.0, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram2.SetUniformMatrix4fv("pvMatrix", camera1.GetPVMatrix());
		shaderProgram2.SetUniformMatrix4fv("modelMatrix", modelMatrix);
		shaderProgram2.UseProgram();

		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		window2.UpdateDisplay();

		camera1.Move(glm::vec3{ 0.001f });
		camera1.RecalculateMatrices();

		glfwPollEvents();
	}
}

