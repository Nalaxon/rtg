


#ifndef INCLUDED_RENDERER
#define INCLUDED_RENDERER

#pragma once

#include <GL/platform/Renderer.h>
#include <GL/platform/Window.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <time.h>
#include <vector>

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

	GLuint programmId;
	GLuint vertexShaderId;
	GLuint fragmentShaderId;
	GLuint vaoId;
	GLuint structureVertexId;
	GLuint structureUvId;
	GLuint colorBufferId;
	GLuint normalId;
	GLuint indexId;
	GLuint lightId;
	GLuint textureId;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	glm::mat4 modelMatrix;
	GLint uniMVP;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	Camera camera;
	View view;
	clock_t lastTime;
	float rotateAngle;
	void  CheckError(const std::string funcName);
	void calculateNormals(const int ind_size, const GLuint* indices, const std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals);
	bool loadOBJ(const char * path, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals);



public:
	Renderer(const Renderer&) = delete;
	Renderer& operator =(const Renderer&) = delete;

	Renderer(GL::platform::Window& window);

	void resize(int width, int height);
	void render();
	void createShader(const char* vertexShaderPath, const char* fragmentShaderPath);
	void destroyShader(void);
	void createStructure(void);
	void destroyStructure(void);
};

#endif  // INCLUDED_RENDERER
