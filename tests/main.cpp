

#include <iostream>

#include <Renderer.h>
#include <Window.h>
#include <ShaderProgram.h>


void InputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	std::cout << "Window " << window << " pressed key " << key;
}

/// <summary>
/// Static buffer setup for test
/// </summary>
/// <returns>ID of VAO we've setup</returns>
int SetupBuffers() {
	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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
	using namespace OORenderer;

	auto window1 = std::make_shared<Window>(800, 600);
	auto window2 = std::make_shared<Window>(800, 600);

	window1->RegisterKeyCallback(InputCallback);
	window2->RegisterKeyCallback(InputCallback);

	// Advanced construction 9arbitrary shader stages as supported by OpenGL
	ShaderProgram shaderProgram1{ window1 };

	shaderProgram1.RegisterShader(std::filesystem::path("./resources/shaders/vertShader.vs"), GL_VERTEX_SHADER);
	shaderProgram1.RegisterShader(std::filesystem::path("./resources/shaders/fragShader.fs"), GL_FRAGMENT_SHADER);

	shaderProgram1.LinkProgram();

	// Simple construction
	ShaderProgram shaderProgram2{ window2, "./resources/shaders/vertShader.vs", "./resources/shaders/fragShader.fs" };


	// We need to be on the correct context when we build the buffers
	window1->ActivateWindow();
	unsigned int VAO1 = SetupBuffers();
	window2->ActivateWindow();
	unsigned int VAO2 = SetupBuffers();

	while (!window1->ShouldClose() && !window2->ShouldClose()) {

		window1->ActivateWindow();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram1.UseProgram();
		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		window1->UpdateDisplay();



		window2->ActivateWindow();

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glClearColor(1.0, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram2.UseProgram();
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		window2->UpdateDisplay();

		glfwPollEvents();
	}
}

