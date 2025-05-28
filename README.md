# OORenderer

## Work In Progress

This project is to tide me over while I'm between jobs, to keep the iron hot, as it were.
It is presented in its work in progress state to demonstrate my process for such projects.
Up to date examples of the usage patterns of the objects in development are available in the tests project.

## Summary

An object oriented renderer with multi window support using GLFW 3.4 and OpenGL 4.6.

## Getting Started

### Setup

This repo may be included as a submodule or one may generate the static library by itself and link it manually to your project.
For use in a CMake project simply add the OORenderer directory to your CMakeLists.txt and then link OORenderer to your desired target like so
>target_link_libraries(YOUR_TARGET $\{OORENDERER_LIBRARY_NAME\})

You may also wish to add the include directory to ease including the available headers where you need them. You may do this like so
>target_include_directories(YOUR_TARGET [PRIVATE|PUBLIC|PROTECTED] $\{OORENDERER_INCLUDE_DIR\})

Please note the helper CMake variables OORENDERER_LIBRARY_NAME and OORENDERER_INCLUDE_DIR are set when calling add_subdirectory() on the OORenderer directory and are set in the calling CMakeLists scope.

### Hello Windows

We'll start by generating two OS standard windows.

Our Window class is defined in `Window.h`. Simply constructing a Window object creates a GLFW window and associates an OpenGL context.
So we may create our two windows like so

```C++
#include <Window.h>

int main() {
	Window window1{800, 600};
	Window window2{800, 600};

	while (!window1.ShouldClose() && !window2.ShouldClose()) {
		// Do stuff while both windows are open
	}
}
```

The standard arguments of glfwCreateWindow are all options for the constructor, with only the width and height being required.
Further window customisation may be done through the use of GLFW window hints before constructing the window object.

To show that we have two seperate OpenGL contexts let us set some clear colours.
When we want to issue OpenGL commands to a window's context we first activate that window by calling the ActivateWindow() method.
We then issue our OpenGL commands, and finally we ask the window to update it's display (we're using the default dual buffer setup) using the UpdateDisplay() method.

Like so
```C++
#include <Window.h>

int main() {
	Window window1{800, 600};
	Window window2{800, 600};

	while (!window1.ShouldClose() && !window2.ShouldClose()) {
		window1.ActivateWindow();
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		window1.UpdateDisplay();

		window2.ActivateWindow();
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		window2.UpdateDisplay();
	}
}
```

Now we should have two windows of differing colours.

### Shaders

So you want to use shaders to display to these windows we've created. We provide the ShaderProgram class via `ShaderProgram.h` for just such a purpose.
Each shader program is relevant to a single OpenGL context, consequently when we create it we need a context to bind it to, we do this by providing the Window we wish this shader program to work on.

Say we have a simple shader program like the one in our test sample, we can setup that shader program like so
```C++
ShaderProgram shaderProgram1{ window1 };
shaderProgram1.RegisterShader(std::filesystem::path("./resources/shaders/vertShader.vs"), GL_VERTEX_SHADER);
shaderProgram1.RegisterShader(std::filesystem::path("./resources/shaders/fragShader.fs"), GL_FRAGMENT_SHADER);
shaderProgram1.LinkProgram();
```

Here we create the program, in window1's context. We then add a vertex shader and a fragment shader to the program - N.B that we provide the file paths as std::filesystem paths, there is an overload which takes const char* as input, though that is assumed to be the literally source not a file path, take care.
Lastly we link the program, this must be done after all shaders have been added to the program. We only support shaders written in GLSL at present.

We also provide the simpler construction method

```C++
ShaderProgram shaderProgram2{ window2, "./resources/shaders/vertShader.vs", "./resources/shaders/fragShader.fs" };
```

Which takes paths to vertext and fragment shaders and registers them with the program and links the program.
