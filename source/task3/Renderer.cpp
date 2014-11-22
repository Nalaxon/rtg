


#include "Renderer.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

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

	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	CheckError("CreateShader(VERTEX)");
	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	CheckError("CreateShader(FRAGMENT)");

	const char* const_vss = vertexShaderSource.c_str();
	const char* const_vfs = fragmentShaderSource.c_str();

	std::cout << "Compile Vertex Shader..." << std::endl;

	glShaderSource(vertexShaderObject, 1, &const_vss, NULL);
	glCompileShader(vertexShaderObject);


	// Check Vertex Shader
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> VertexShaderErrorMessage(infoLogLength);
		glGetShaderInfoLog(vertexShaderObject, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: %s \n", &VertexShaderErrorMessage[0]
			);
		getchar();
		exit(-1);
	}

	std::cout << "Compile Fragment Shader..." << std::endl;

	glShaderSource(fragmentShaderObject, 1, &const_vfs, NULL);
	glCompileShader(fragmentShaderObject);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> FragmentShaderErrorMessage(infoLogLength);
		glGetShaderInfoLog(fragmentShaderObject, infoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: %s \n", &FragmentShaderErrorMessage[0]
			);
		getchar();
		exit(-1);
	}

	std::cout << "Create Program..." << std::endl;

	programmID = glCreateProgram();
	CheckError("CreateProgramm()");

	glAttachShader(programmID, vertexShaderObject);
	CheckError("AttachShader(vertex)");
	glAttachShader(programmID, fragmentShaderObject);
	CheckError("AttachShader(fragment)");

	glLinkProgram(programmID);

	// Check the program
	glGetProgramiv(programmID, GL_LINK_STATUS, &result);
	if (result != GL_TRUE)
	{
		glGetProgramiv(programmID, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> ProgramErrorMessage(std::max(infoLogLength, int(1)));
		glGetProgramInfoLog(programmID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(
			stderr,
			"ERROR: Could not link programm: %s \n", &ProgramErrorMessage[0]
			);
		getchar();
		exit(-1);
	}

	glUseProgram(programmID);
	CheckError("UseProgramm()");

}

void Renderer::destroyShader(void)
{
	glUseProgram(0);
	CheckError("UserProgram(0)");
	glDetachShader(programmID, vertexShaderObject);
	CheckError("DetachShader(, vertexShader)");
	glDetachShader(programmID, fragmentShaderObject);
	CheckError("DetachShader(, framentShader)");
	glDeleteShader(fragmentShaderObject);
	CheckError("DeleteShader(fragmentShader)");
	glDeleteShader(vertexShaderObject);
	CheckError("DeleteShader(vertexShader)");
	glDeleteProgram(programmID);
	CheckError("DelteProgramm()");
}

void Renderer::createNaiveStructure(void)
{
	//VAO
	glGenVertexArrays(1, &vaoId);
	CheckError("GenVertexArrays(1, vaoId");
	glBindVertexArray(vaoId);
	CheckError("BindVertexArray(vaoId)");

	float a = (1 + std::sqrt(5.0f)) / 2;
	//dodecahedron
	GLfloat structure[] = {
		-.1f, -.1f, .1f,
		-.1f, .1f, .1f,
		.1f, .1f, .1f,
		.1f, -.1f, .1f,
		-.1f, -.1f, -.1f,
		-.1f, .1f, -.1f,
		.1f, .1f, -.1f,
		.1f, -.1f, -.1f,
	
		//basic quader
		-1.0f, 1.0f, 1.0f,     //foreground
		-1.0f, -1.0f, -1.0f,
		 1.0f, 1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,     //background
		-1.0f, -1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f,

		 //edge fore and background
		 0.0f, 1/a, -a,
		 0.0f, -1/a, -a,
		 0.0f, 1/a, a,
		 0.0f, -1/a, a,

		 //edges top buttom
		 -1/a, a, 0.0f,
		 1/a, a, 0.0f,
		 -1/a, -a, 0.0f,
		 1/a, -a, 0.0f,

		 //edges left right
		 -a, 0.0f, -1/a,
		 -a, 0.0f, 1/a,
		 a, 0.0f, -1/a,
		 a, 0.0f, 1/a
		 
	};
	
	const GLuint indices[] = {
		0,2,1,  0,3,2,
		4,3,0,  4,7,3,
		4,1,5,  4,0,1,
		3,6,2,  3,7,6,
		1,6,5,  1,2,6,
		7,5,6,  7,4,5
	};

	glGenBuffers(2, &vStructureId);
	CheckError("GenBuffers(2, StructureId)");
	glBindBuffer(GL_ARRAY_BUFFER, vStructureId);
	CheckError("BindBuffer(Structre)");
	glBufferData(GL_ARRAY_BUFFER, sizeof(structure), structure, GL_STATIC_DRAW);
	CheckError("Bufferdata()");
	glEnableVertexAttribArray(0);
	CheckError("EnableVertexAttriArray(0)");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	CheckError("VertexAttribPointer(0, 3,...)");

	
	//glGenBuffers(3, &vBufferId);
	//CheckError("GenBuffers(3, BufferId)");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vBufferId);
	CheckError("BindBuffer(Indices)");
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	CheckError("BufferData(indices)");

	//initial camera position and orientation
	camera.p_camera = glm::vec3(0.0f, 0.0f, -0.2f);
	camera.p_lookat = glm::vec3(0.0f, 0.0f, 0.0f);
	camera.v_up = glm::vec3(0.0f, 1.0f, 1.0f);

	//init view pyramid
	view.beta = 60;
	view.z_n = 0.1f;
	view.z_f = 400;
	view.aspect = static_cast<float>(viewport_width / viewport_height);
	view.h = -2 * view.z_n * tan(view.beta / 2);

	//Calculate of matrices
	ProjectionMatrix = {1/ (view.aspect * view.h), 0, 0, 0,
	                             0, 1/view.h, 0, 0,
	                             0, 0, - (view.z_f + view.z_n) / (view.z_f - view.z_n), -1,
	                             0, 0, - (2 * view.z_f * view.z_n) / (view.z_f - view.z_n), 0};

	ViewMatrix =  glm::lookAt(
		camera.p_camera,
		camera.p_lookat,
		camera.v_up
		);

	ModelMatrix = {1, 0, 0, 0,
	               0, 1, 0, 0,
	               0, 0, 1, 0,
	               0, 0, 0, 1};

	//upload matrix to shader
	GLint uniTrans = glGetUniformLocation(programmID, "proj");
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

	GLint uniView = glGetUniformLocation(programmID, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(ViewMatrix));

	GLint uniModel = glGetUniformLocation(programmID, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
}

void Renderer::destroyNaiveStructure(void)
{

}

void Renderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, viewport_width, viewport_height);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
	
	//glTranslated(-3.0, 1.5, -6);
	//glScaled(0.5, 0.5, 0.5);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	
	context.swapBuffers();
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
