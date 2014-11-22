


#ifndef INCLUDED_RENDERER
#define INCLUDED_RENDERER

#pragma once

#include <GL/platform/Renderer.h>
#include <GL/platform/Window.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

typedef struct {
	glm::vec3 v_up;
	glm::vec3 p_camera;
	glm::vec3 p_lookat;
} Camera;

typedef struct {
	float beta;
	float z_n;
	float z_f;
	float h;
	float w;
	float aspect;
} View;

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
	GLuint vStructureId;
	GLuint vBufferId;
	GLuint ColorBufferId;
	glm::mat4 ModelViewProjectionMatrix;
	Camera camera;
	View view;
	void  CheckError(const std::string funcName);

public:
	Renderer(const Renderer&) = delete;
	Renderer& operator =(const Renderer&) = delete;

	Renderer(GL::platform::Window& window);

	void resize(int width, int height);
	void render();
	void createShader(const char* vertexShaderPath, const char* fragmentShaderPath);
	void destroyShader(void);
	void createNaiveStructure(void);
	void destroyNaiveStructure(void);
};

#endif  // INCLUDED_RENDERER
