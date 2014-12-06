


#include <iostream>
#include <stdexcept>

#include <GL/platform/Window.h>

#include "Renderer.h"
#include "InputHandler.h"

void APIENTRY OpenGLDebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cout << message << '\n';
}

int main(int argc, char* argv[])
{
	const char* VERTEX_SHADER_SOURCE = ".\\..\\..\\..\\..\\source\\task4\\vertexshader.glsl";
	const char* FRAGMENT_SHADER_SOURCE = ".\\..\\..\\..\\..\\source\\task4\\fragmentshader.glsl";
	//const char* VERTEX_SHADER_SOURCE = "./source/task3/vertexshader.glsl";
	//const char* FRAGMENT_SHADER_SOURCE = "./source/task3/fragmentshader.glsl";
	try
	{
		GL::platform::Window window("Assignment 4 — Complex 3D object", 800, 600, 24, 8, false, 4, 3);
		Renderer renderer(window);
		

		glDebugMessageCallback(OpenGLDebugOutputCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

		renderer.resize(800, 600);

		InputHandler input_handler;

		renderer.createShader(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
		renderer.createStructure();

		window.attach(static_cast<GL::platform::KeyboardInputHandler*>(&input_handler));
		window.attach(static_cast<GL::platform::MouseInputHandler*>(&input_handler));

		GL::platform::run(renderer);

		renderer.destroyStructure();
		renderer.destroyShader();
	}
	catch (std::exception& e)
	{
		std::cout << "error: " << e.what() << std::endl;
		getchar();
		return -1;
	}
	catch (...)
	{
		std::cout << "unknown exception" << std::endl;
		getchar();
		return -128;
	}

	return 0;
}
