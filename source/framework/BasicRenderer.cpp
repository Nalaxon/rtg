


#include "BasicRenderer.h"


BasicRenderer::BasicRenderer(GL::platform::Window& window)
	: context(window)
{
	window.attach(this);
}

void BasicRenderer::resize(int width, int height)
{
}

void BasicRenderer::render()
{
	swapBuffers();
}

void BasicRenderer::swapBuffers()
{
	context.swapBuffers();
}
