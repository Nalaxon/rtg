#include <iostream>
#include <stdexcept>

#include <GL/platform/Window.h>

#include "Renderer.h"
#include "InputHandler.h"


int main(int argc, char* argv[])
{
    const char* VERTEX_SHADER_SOURCE = ".\\..\\..\\..\\..\\source\\task2\\vertexshader.glsl";
	const char* FRAGMENT_SHADER_SOURCE = ".\\..\\..\\..\\..\\source\\task2\\fragmentshader.glsl";
	//const char* VERTEX_SHADER_SOURCE = "./source/task2/vertexshader.glsl";
	//const char* FRAGMENT_SHADER_SOURCE = "./source/task2/fragmentshader.glsl";
	try
	{
		GL::platform::Window window("Assignment 2 — More complex shapes", 800, 600, 0, 0, false, 3, 3);

		Renderer renderer(window);
		renderer.resize(800, 600);

		renderer.createShader(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
		renderer.createNaivePolygon();

		InputHandler input_handler;

		window.attach(static_cast<GL::platform::KeyboardInputHandler*>(&input_handler));
		window.attach(static_cast<GL::platform::MouseInputHandler*>(&input_handler));
		
		GL::platform::run(renderer);

		renderer.destroyNaivePolygon();
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
