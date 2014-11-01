


#ifndef INCLUDED_RENDERER
#define INCLUDED_RENDERER

#pragma once

#include <GL/platform/Renderer.h>
#include <GL/platform/Window.h>


class Renderer : public virtual GL::platform::Renderer, public virtual GL::platform::DisplayHandler
{
private:
	GL::platform::scoped_context context;

	int viewport_width;
	int viewport_height;

	GLuint programmID;
	GLuint vertexShaderObject;
	GLuint fragmentShaderObject;
	GLuint vaoId;
	GLuint vTriangleId;

public:
	Renderer(const Renderer&) = delete;
	Renderer& operator =(const Renderer&) = delete;

	Renderer(GL::platform::Window& window);

	void resize(int width, int height);
	void render();
	void createShader(const char* vertexShaderPath, const char* fragmentShaderPath);
	void destroyShader(void);
	void createVertexBuffers(void);
	void destroyVertexBuffers(void);
};

#endif  // INCLUDED_RENDERER
