


#include "Renderer.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>

static float PI = 3.14159265358979323846f;

Renderer::Renderer(GL::platform::Window& window)
	: context(window)
{
	glClearColor(0.1f, 0.3f, 1.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	window.attach(this);
}

void Renderer::resize(int width, int height)
{
	viewport_width = width;
	viewport_height = height;
}

void Renderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, viewport_width, viewport_height);

	context.swapBuffers();
}

void loadshader(const char* filename, std::string &ShaderSource)
{
	std::ifstream file;


	std::ifstream ShaderStream(filename, std::ios::in);
	if (ShaderStream.is_open())
	{
		std::string Line = "";
		while (std::getline(ShaderStream, Line))
			ShaderSource += "\n" + Line;
		ShaderStream.close();
	}

}

int unloadshader(GLubyte** ShaderSource)
{
	if (*ShaderSource != 0)
		delete[] * ShaderSource;
	*ShaderSource = 0;
	return 0;
}

void Renderer::createShader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	GLint result = GL_FALSE;
	//GLenum ErrorCheckValue;
	int infoLogLength;
	std::string vertexShaderSource, fragmentShaderSource;

	loadshader(vertexShaderPath, vertexShaderSource);
	loadshader(fragmentShaderPath, fragmentShaderSource);

	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	CheckError("CreateShader(VERTEX)");
	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	CheckError("CreateShader(FRAGMENT)");

	const char* const_vss = vertexShaderSource.c_str();
	const char* const_vfs = fragmentShaderSource.c_str();

	std::cout << "Compile Vertex Shader..." << std::endl;

	glShaderSource(vertexShaderId, 1, &const_vss, NULL);
	glCompileShader(vertexShaderId);


	// Check Vertex Shader
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> VertexShaderErrorMessage(infoLogLength);
		glGetShaderInfoLog(vertexShaderId, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: %s \n", &VertexShaderErrorMessage[0]
			);
		getchar();
		exit(-1);
	}

	std::cout << "Compile Fragment Shader..." << std::endl;

	glShaderSource(fragmentShaderId, 1, &const_vfs, NULL);
	glCompileShader(fragmentShaderId);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> FragmentShaderErrorMessage(infoLogLength);
		glGetShaderInfoLog(fragmentShaderId, infoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: %s \n", &FragmentShaderErrorMessage[0]
			);
		getchar();
		exit(-1);
	}

	std::cout << "Create Program..." << std::endl;

	programmId = glCreateProgram();
	CheckError("CreateProgramm()");

	glAttachShader(programmId, vertexShaderId);
	CheckError("AttachShader(vertex)");
	glAttachShader(programmId, fragmentShaderId);
	CheckError("AttachShader(fragment)");

	glLinkProgram(programmId);

	// Check the program
	glGetProgramiv(programmId, GL_LINK_STATUS, &result);
	if (result != GL_TRUE)
	{
		glGetProgramiv(programmId, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> ProgramErrorMessage(std::max(infoLogLength, int(1)));
		glGetProgramInfoLog(programmId, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(
			stderr,
			"ERROR: Could not link programm: %s \n", &ProgramErrorMessage[0]
			);
		getchar();
		exit(-1);
	}

	glUseProgram(programmId);
	CheckError("UseProgramm()");

}

void Renderer::destroyShader(void)
{
	glUseProgram(0);
	CheckError("UserProgram(0)");
	glDetachShader(programmId, vertexShaderId);
	CheckError("DetachShader(, vertexShader)");
	glDetachShader(programmId, fragmentShaderId);
	CheckError("DetachShader(, framentShader)");
	glDeleteShader(fragmentShaderId);
	CheckError("DeleteShader(fragmentShader)");
	glDeleteShader(vertexShaderId);
	CheckError("DeleteShader(vertexShader)");
	glDeleteProgram(programmId);
	CheckError("DelteProgramm()");
}

void Renderer::createStructure(void)
{
	//VAO
	glGenVertexArrays(1, &vaoId);
	CheckError("GenVertexArrays(1, vaoId");
	glBindVertexArray(vaoId);
	CheckError("BindVertexArray(vaoId)");

	std::vector<glm::vec3> structure;

}

void Renderer::destroyStructure(void)
{

}

void Renderer::calculateNormals(const int ind_size, const GLuint* indices, const std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals)
{
	static int offset = 3;

	glm::fvec3 v1, v2, v3, a, b, cross;

	//std::cout << "size of indices: " << ind_size << "sizeof(unsigned int) " << sizeof(GLuint) <<  "  size of vertices: " << vertices.size() << std::endl;

	for (unsigned int i = 0; (i < ind_size / sizeof(GLuint)); i += offset)
	{

		v1 = vertices[indices[i]];
		v2 = vertices[indices[i + 1]];
		v3 = vertices[indices[i + 2]];
		a = v2 - v1;
		b = v3 - v2;

		cross = glm::cross(a, b);
		cross = glm::normalize(cross);

		//std::cout << "v1(" << v1.x << "," << v1.y << "," << v1.z << ") / v2(";
		//std::cout << v2.x << "," << v2.y << "," << v2.z << ") / v3(";
		//std::cout << v3.x << "," << v3.y << "," << v3.z << ")" << std::endl;

		//std::cout << "a(" << a.x << "," << a.y << "," << a.z << ") / b(" << b.x << "," << b.y << "," << b.z << ")" << std::endl;
		//std::cout << "X(" << cross.x << "," << cross.y << "," << cross.z << std::endl;

		normals.push_back(cross);  //overtake normalised norm
		if (i % 2 == 0)
		{
			normals.push_back(cross);
			normals.push_back(cross);
		}

	}
}

void  Renderer::CheckError(const std::string funcName)
{
	GLenum ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		std::cerr << "ERROR: While function " << funcName << std::endl;
		getchar();
		exit(-1);
	}
}
