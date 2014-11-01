


#ifndef INCLUDED_FRAMEWORK_BASIC_RENDERER
#define INCLUDED_FRAMEWORK_BASIC_RENDERER

#pragma once

#include <GL/platform/Renderer.h>
#include <GL/platform/Window.h>


class BasicRenderer : public virtual GL::platform::Renderer, public virtual GL::platform::DisplayHandler
{
private:
	GL::platform::scoped_context context;

protected:
	void swapBuffers();

public:
	BasicRenderer(const BasicRenderer&) = delete;
	BasicRenderer& operator =(const BasicRenderer&) = delete;

	BasicRenderer(GL::platform::Window& window);

	virtual void resize(int width, int height);
	virtual void render();
};

#endif  // INCLUDED_FRAMEWORK_BASIC_RENDERER
